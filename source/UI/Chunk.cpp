#include "Chunk.h"
#include "Block.h"
#include "General.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include <algorithm>
namespace MyCraft {

    Chunk::Chunk(): __bitOn(0) {
        __blocks.resize(16);
        for (int i = 0; i<16; i++) {
            __blocks[i].resize(16);
            for (int j = 0; j<16; j++) {
                __blocks[i][j].resize(16);
                for (int k = 0; k<16; k++) {
                    __blocks[i][j][k] = BlockCatogary::Air;
                }
            }
        }
    }
    Chunk::~Chunk() {

    }
    bool Chunk::contains(const glm::vec3& center) const {
        return (center.x >= __position.x && center.x <= __position.x+16 &&
                center.y >= __position.y && center.y <= __position.y+16 &&
                center.z >= __position.z && center.z <= __position.z+16) ;
    }

    bool Chunk::setHover(const MyBase3D::Ray3f& ray) {
        bool hover = false;
        glm::vec3 delta = ray;
        float rX = ceil(ray.getOrigin().x) - ray.getOrigin().x;
        rX /= delta.x;
        float rY = ceil(ray.getOrigin().y) - ray.getOrigin().y;
        rY /= delta.y;
        float rZ = ceil(ray.getOrigin().z) - ray.getOrigin().z;
        rZ /= delta.z;
        float rMin = std::min(rX, std::min(rY, rZ));
        delta *= rMin;
        return hover;
    }

    const BlockCatogary::Catogary& Chunk::at(const int& x, const int& y, const int& z)  const{
        return __blocks[x][y][z];
    }
    void Chunk::__enableBit(const int& x, const int& y, const int& z) {
        if (!__bits[x][y][z]) {
            __bitOn++;
            __horizontalPlane[z]++;
            __list[__blocks[x][y][z]].push_back({x+__position.x,y+__position.y,z+__position.z,1});
            __bits[x][y][z] = 1;
        }
    }
    void Chunk::__disableBit(const int& x, const int& y, const int& z) {
        if (!__bits[x][y][z]) return;
        if (x>0 && !__blocks[x-1][y][z]) ;
        else if (x<14 && !__blocks[x+1][y][z]) ;
        else if (y>0 && !__blocks[x][y-1][z]) ;
        else if (y<14 && !__blocks[x][y+1][z]) ;
        else if (z>0 && !__blocks[x][y][z-1]) ;
        else if (z<14 && !__blocks[x][y][z+1]) ;
        else if (z<15 && z>0 && x<15 && x>0 && y<15 && y>0) {
            __bits[x][y][z] = 0;
            __bitOn--;
            __horizontalPlane[z]--;
            glm::vec4 pos(x+__position.x, y+__position.y, z+__position.z ,1);
            auto& tmp = __list[__blocks[x][y][z]];
            int i = 0;
            while (tmp[i]!=pos) i++;
            tmp.erase(tmp.begin()+i);
        }
    }
    void Chunk::set(const int& x, const int& y, const int& z, const BlockCatogary::Catogary& type) {
        if (!type) {
            if (__blocks[x][y][z]) {
                __disableBit(x, y, z);
                __blocks[x][y][z] = BlockCatogary::Catogary::Air;
                if (x>0 && !__bits[x-1][y][z] && __blocks[x-1][y][z]) __enableBit(x-1, y, z);
                if (x<15 && !__bits[x+1][y][z] && __blocks[x+1][y][z]) __enableBit(x+1, y, z);

                if (y>0 && !__bits[x][y-1][z] && __blocks[x][y-1][z]) __enableBit(x, y-1, z);
                if (y<15 && !__bits[x][y+1][z] && __blocks[x][y+1][z]) __enableBit(x, y+1, z);
                
                if (z>0 && !__bits[x][y][z-1] && __blocks[x][y][z-1]) __enableBit(x, y, z-1);
                if (z<15 && !__bits[x][y][z+1] && __blocks[x][y][z+1]) __enableBit(x, y, z+1);
            }
        }
        else {
            if (!__blocks[x][y][z]) {
                __blocks[x][y][z] = type;
                __enableBit(x, y, z);
                if (x>0 && __bits[x-1][y][z]) __disableBit(x-1, y, z);
                if (x<15 && __bits[x+1][y][z]) __disableBit(x+1, y, z);

                if (y>0 && __bits[x][y-1][z]) __disableBit(x, y-1, z);
                if (y<15 && __bits[x][y+1][z]) __disableBit(x, y+1, z);
                
                if (z>0 && __bits[x][y][z-1]) __disableBit(x, y, z-1);
                if (z<15 && __bits[x][y][z+1]) __disableBit(x, y, z+1);
            }
            else {
                glm::vec4 pos(x+__position.x, y+__position.y, z+__position.z ,1);
                auto& tmp = __list[__blocks[x][y][z]];
                int i = 0;
                while (i<tmp.size() && tmp[i]!=pos) i++;
                if (i<tmp.size()) {
                    tmp.erase(tmp.begin()+i);
                    __blocks[x][y][z] = type;
                    __list[__blocks[x][y][z]].push_back(pos);
                }
            }
        }
    }
    void Chunk::setPosition(const int& x, const int& y, const int& z) {
        __position = {x, y, z};
    }
    void Chunk::setPosition(const glm::vec3& position) {
        setPosition(position.x, position.y, position.z);
    }

    void Chunk::glDraw(const glm::vec3& position, const glm::vec3& direction) const {
        if (!__bitOn) return ;
        GLuint VAO;
        std::vector<GLuint> POS(3);
        glUseProgram(MyBase3D::ShaderStorage::Default->GetCubeShader());
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getImageBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::Default->getBlockSet());
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, MyCraft::BlockCatogary::Default->getTexCoord());

        glActiveTexture(GL_TEXTURE0);

        glGenBuffers(POS.size(), POS.data());
        for (int i = 0; i<3; i++) {
            glBindBuffer(GL_UNIFORM_BUFFER, POS[i]);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*4*32, 0, GL_DYNAMIC_DRAW);
        }
        for (const auto& item:__list) {
            const auto& poss = item.second;
        
            glBindTexture(GL_TEXTURE_2D, MyCraft::BlockCatogary::Default->getBlock(item.first));

            int curr = 0;
            for (int i = 0; i<poss.size(); i+=32) {
                int sz = std::min(32, (int)poss.size()-i);
                glBindBuffer(GL_UNIFORM_BUFFER, POS[curr]);
                glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(GLfloat)*4*sz, &poss[i]);
                glBindBufferBase(GL_UNIFORM_BUFFER, 1, POS[curr]);    
                glDrawArrays(GL_TRIANGLES, 0, 36*sz);
                curr = (curr+1)%3;
            }
        }
        glDeleteBuffers(POS.size(), POS.data());
        glDeleteVertexArrays(1, &VAO);
    }
}
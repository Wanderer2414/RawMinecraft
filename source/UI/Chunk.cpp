#include "Chunk.h"
#include "Block.h"
#include "General.h"
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

const unsigned char& Chunk::at(const int& x, const int& y, const int& z)  const{
    return __blocks[x][y][z];
}
void Chunk::__enableBit(const int& x, const int& y, const int& z) {
    if (!__bits[x][y][z]) {
        __bitOn++;
        __horizontalPlane[z]++;
    }
    __bits[x][y][z] = 1;
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
    }
}
void Chunk::set(const int& x, const int& y, const int& z, const BlockCatogary::Catogary& type) {
    if (!type) {
        if (__blocks[x][y][z]) {
            __blocks[x][y][z] = 0;
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
        else __blocks[x][y][z] = type;
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
    DrawMargin(__position, glm::vec3(16,16,16), glm::vec3(1,0,0));
    GLuint VAO, POS;
    BindCube(VAO, POS);
    for (int k = 0; k<16; k++) {
        if (!__horizontalPlane[k]) continue;
        for (int i = 0; i<16; i++) {
            for (int j = 0; j<16; j++) {
                if (!__bits[i][j].any()) continue;
                if (__bits[i][j][k]) {
                    DrawCube(POS, __blocks[i][j][k], __position+glm::vec3(i,j,k));
                }
            }
        }
    }
    FreeCube(VAO, POS);
}
}
#include "Chunk.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"

Chunk::Chunk() {
    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            for (int k = 0; k<16; k++) {
                blocks[i][j][k].type = BlockCatogary::Air;
                blocks[i][j][k].setPosition(pPosition.x+i, pPosition.y+j, pPosition.z+k);
            }
        }
    }
}
Chunk::~Chunk() {

}
bool Chunk::contains(const glm::vec3& center) const {
    return (center.x >= pPosition.x && center.x <= pPosition.x+16 &&
            center.y >= pPosition.y && center.y <= pPosition.y+16 &&
            center.z >= pPosition.z && center.z <= pPosition.z+16) ;
}

bool Chunk::setHover(const Ray3f& ray) {
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

Block& Chunk::at(const int& x, const int& y, const int& z) {
    return blocks[x][y][z];
}

void Chunk::setPosition(const int& x, const int& y, const int& z) {
    pPosition = {x, y, z};
    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            for (int k = 0; k<16; k++) {
                blocks[i][j][k].setPosition(pPosition.x+i, pPosition.y+j, pPosition.z+k);
            }
        }
    }
}
void Chunk::setPosition(const glm::vec3& position) {
    setPosition(position.x, position.y, position.z);
}

void Chunk::glDraw() const {
    glLineWidth(2);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, PointSet::Default->ChunkSet);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    GLuint Origin;

    float buffer[8] = {
        pPosition.x, pPosition.y, pPosition.z, 0,
        1, 0, 0, 1
    };
    glGenBuffers(1, &Origin);
    glBindBuffer(GL_UNIFORM_BUFFER, Origin);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*8, buffer, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, Origin);

    glUseProgram(ShaderStorage::Default->ChunkShader);
    glDrawArrays(GL_LINE_STRIP, 0, 16);
    
    glDeleteBuffers(1, &Origin);
    glDeleteVertexArrays(1, &VAO);

    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            for (int k = 0; k<16; k++) {
                if (blocks[i][j][k].type != BlockCatogary::Air) {
                    blocks[i][j][k].glDraw();
                }
            }
        }
    }
}
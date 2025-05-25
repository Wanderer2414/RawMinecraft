#include "Chunk.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"

Chunk::Chunk() {
    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            for (int k = 0; k<16; k++) {
                type_store[i][j][k] = BlockCatogary::Air;
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

char& Chunk::at(const int& x, const int& y, const int& z) {
    return type_store[x][y][z];
}

void Chunk::setPosition(const int& x, const int& y, const int& z) {
    pPosition = {x, y, z};
}
void Chunk::setPosition(const glm::vec3& position) {
    pPosition = position;
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
                if (type_store[i][j][k] != BlockCatogary::Air) {
                    drawBlock(i, j, k);
                }
            }
        }
    }
}
void Chunk::drawBlock(const int& x, const int& y, const int& z) const {
    glUseProgram(ShaderStorage::Default->CubeShader);
    glm::vec3 position = glm::vec3(x, y, z) + pPosition;
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, PointSet::Default->BlockSet);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, BlockCatogary::Default->BlockTexture);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(1);

    GLuint originPoint;
    
    glGenBuffers(1, &originPoint);
    glBindBuffer(GL_UNIFORM_BUFFER, originPoint);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*3, &position[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BlockCatogary::Default->getBlock(type_store[x][y][z]));

    glDrawArrays(GL_QUADS, 0, 24);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &originPoint);
}
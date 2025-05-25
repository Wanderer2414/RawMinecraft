#include "Chunk.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "SFML/Graphics/Shader.hpp"
#include "ShaderStorage.h"
#include <GL/gl.h>

Chunk::Chunk() {

}
Chunk::~Chunk() {

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
}
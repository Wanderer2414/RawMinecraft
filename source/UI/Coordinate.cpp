#include "Coordinate.h"
#include "ShaderStorage.h"
#include <GL/gl.h>
#include <GL/glext.h>

Coordinate::Coordinate() {

}
Coordinate::~Coordinate() {

}

void Coordinate::draw(RenderTarget& target, RenderStates state) const {
}
void Coordinate::glDraw() const {
    glLineWidth(3);
    GLuint VAO, BAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &BAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, BAO);
    glm::vec3 vertices[84];
    for (int i = 0; i<10; i++) {
        vertices[i*4] = glm::vec3(-5, i- 5, 0);
        vertices[i*4+1] = {1, 0, 0};
        vertices[i*4+2] = glm::vec3(+5, i - 5, 0);
        vertices[i*4+3] = {1, 0, 0};
    }
    for (int i = 0; i<10; i++) {
        vertices[40 + i*4] = glm::vec3(i-5, -5, 0);
        vertices[40 + i*4+1] = {0, 1, 0};
        vertices[40 + i*4+2] = glm::vec3(i-5, 5, 0);
        vertices[40 + i*4+3] = {0, 1, 0};
    }
    vertices[80] = {0, 0, -5};
    vertices[81] = {0, 0, 1};
    vertices[82] = {0, 0, 5};
    vertices[83] = {0, 0, 1};

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*84*3, &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);

    glUseProgram(ShaderStorage::Default->DefaultShader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 42);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &BAO);
}
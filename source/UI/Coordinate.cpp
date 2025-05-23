#include "Coordinate.h"
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
    glm::vec3 vertices[12] {
        {0, 0, -10}, {0,0, 1}, {0, 0, 10}, {0, 0, 1},
        {0, -10, 0}, {0, 1, 0}, {0, 10, 0}, {0, 1, 0},
        {-10, 0, 0}, {1,0,0}, {10, 0, 0}, {1, 0, 0}
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*12, &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &BAO);
}
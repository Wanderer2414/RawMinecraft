#include "Cursor.h"
#include "ControlCenter.h"
#include "ShaderStorage.h"
namespace MyCraft {
    Cursor::Cursor() {}
    Cursor::~Cursor() {}

    glm::vec2 Cursor::getPosition() const {
        return {0,0};
    }
    glm::vec2 Cursor::getSize() const {
        return {0,0};
    }
    bool Cursor::contains(const glm::vec2& pos) const {
        return false;
    }
    void Cursor::glDrawTransparent() const {

        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetPoint2DShader());
        GLuint VAO, Positions, Colors, ORI;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
            
        glm::vec2 __direction[4] = {{-0.05/MyBase::ControlCenter::getInstance().GetWindowRatio(), 0}, {0.05/MyBase::ControlCenter::getInstance().GetWindowRatio(), 0}, {0, -0.05}, {0, 0.05}};
        glGenBuffers(1, &Positions);
        glBindBuffer(GL_ARRAY_BUFFER, Positions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*4, &__direction[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &ORI);
        glBindBuffer(GL_UNIFORM_BUFFER, ORI);
        glm::vec2 Pos(0,0);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &Pos, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, ORI);

        glGenBuffers(1, &Colors);
        glBindBuffer(GL_UNIFORM_BUFFER, Colors);
        glm::vec4 color(0,1,0, 1);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &color, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, Colors);
        glLineWidth(4);

        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &ORI);
        glDeleteBuffers(1, &Positions);
        glDeleteBuffers(1, &Colors);
    }
}
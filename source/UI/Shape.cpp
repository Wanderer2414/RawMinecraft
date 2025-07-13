#include "Shape.h"
#include "Global.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"

namespace MyBase {
    Shape::Shape(): __VBO(0) {
        glGenVertexArrays(1, &__VAO);
    }
    Shape::~Shape() {
        if (__VBO) glDeleteBuffers(1, &__VBO);
        glDeleteVertexArrays(1, &__VAO);
    }

    void Shape::update() {
        if (!__VBO) {
            glGenBuffers(1, &__VBO);
        }
        glBindBuffer(GL_ARRAY_BUFFER, __VBO);
        char *buffer = new char[sizeof(glm::vec2)*getPointCount()];
        glm::vec2 *positions = (glm::vec2*)buffer;
        for (int i = 0; i<getPointCount(); i++) {
            positions[i] = getPoint(i);
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*getPointCount(), buffer, GL_STATIC_DRAW);
        glBindVertexArray(__VAO);
        glBindBuffer(GL_ARRAY_BUFFER, __VBO);
        glEnableVertexArrayAttrib(__VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] buffer;
    }
    void Shape::draw() const {
        glBindVertexArray(__VAO);
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getPoint2DShader());
        glDrawArrays(GL_TRIANGLE_FAN, 0, getPointCount());
        glBindVertexArray(0);
    }

    ShapeContainer::ShapeContainer(): __position(0,0), __color(WHITE) {
        glGenBuffers(1, &__positionCode);
        glBindBuffer(GL_UNIFORM_BUFFER, __positionCode);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__position, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        __colorCode = ShapeManager::getInstance().createColor(__color);
    }
    ShapeContainer::~ShapeContainer() {
        ShapeManager::getInstance().removeColor(__color);
        glDeleteBuffers(1, &__positionCode);
    }
    
    Color ShapeContainer::getColor() const {
        return __color;
    }
    glm::vec2 ShapeContainer::getPosition() const {
        return __position;
    }
    void ShapeContainer::setPosition(const glm::vec2& position) {
        if (position != __position) {
            __position = position;
            glBindBuffer(GL_UNIFORM_BUFFER, __positionCode);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__position);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }
    void ShapeContainer::move(const glm::vec2& offset) {
        glm::vec2 pos = __position + offset;
        setPosition(pos);
    }
    void ShapeContainer::setFillColor(const Color& color) {
        if (color != __color) {
            ShapeManager::getInstance().removeColor(__color);
            __color = color;
            __colorCode = ShapeManager::getInstance().createColor(__color);
        }
    }
    
    void ShapeContainer::draw(const Shape& shape) const {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getPoint2DShader());
        glBindVertexArray(shape.__VAO);
        glBindBuffer(GL_ARRAY_BUFFER, shape.__VBO);
        
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __positionCode);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, __colorCode);
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, shape.getPointCount());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
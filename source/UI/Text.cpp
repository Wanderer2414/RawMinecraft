#include "Text.h"
#include "General.h"
#include "Global.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"
namespace MyBase {
    Text::Text(): __font(0), __color(BLACK), __position(0, 0), __scale(1,1) {
        glGenVertexArrays(1, &__VAO);
        
        glGenBuffers(1, &__POSITION);
        glBindBuffer(GL_UNIFORM_BUFFER, __POSITION);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__position, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__SCALE);
        glBindBuffer(GL_UNIFORM_BUFFER, __SCALE);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__scale, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        __COLOR = ShapeManager::getInstance().createColor(__color);
    }
    Text::Text(const Text& text): __color(WHITE), __position(text.__position), __scale(text.__scale) {
        glGenVertexArrays(1, &__VAO);

        glGenBuffers(1, &__POSITION);
        glBindBuffer(GL_UNIFORM_BUFFER, __POSITION);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__position, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__SCALE);
        glBindBuffer(GL_UNIFORM_BUFFER, __SCALE);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__scale, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        __COLOR = ShapeManager::getInstance().createColor(__color);
        setTextColor(text.__color);
        setFont(*text.__font);
    }
    Text::~Text() {
        glDeleteVertexArrays(1, &__VAO);
        glDeleteBuffers(1, &__POSITION);
        glDeleteBuffers(1, &__SCALE);
        ShapeManager::getInstance().removeColor(__color);
    }
    Text& Text::operator=(const Text& text) {
        setPosition(text.__position);
        setScale(text.__scale);
        setTextColor(text.__color);
        setFont(*text.__font);
        return *this;
    }
    glm::vec2 Text::getPosition() const {
        return __position;
    }
    const std::string& Text::getText() const {
        return __text;
    }
    void Text::setScale(const glm::vec2& scale) {
        if (__scale!=scale) {
            __scale = scale;
            glBindBuffer(GL_UNIFORM_BUFFER, __SCALE);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__scale);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }
    void Text::setTextColor(const Color& color) {
        if (__color != color) {
            ShapeManager::getInstance().removeColor(__color);
            __color = color;
            __COLOR = ShapeManager::getInstance().createColor(__color);
        }
    }
    void Text::setFont(const Font& font) {
        __font = (Font*)&font;
        update();
    }
    void Text::setText(const std::string& text) {
        __text = text;
        update();
    }
    const Font& Text::getFont() const {
        return *__font;
    }
    void Text::move(const glm::vec2& offset) {
        setPosition(__position + offset);
    }
    void Text::setPosition(const glm::vec2& position) {
        if (position != __position) {
            __position = position;
            glBindBuffer(GL_UNIFORM_BUFFER, __POSITION);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &position);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }
    void Text::update() {
        if (!__font) return ;
        glDeleteBuffers(1, &__VBO);
        glGenBuffers(1, &__VBO);
        
        glBindVertexArray(__VAO);
        glBindBuffer(GL_ARRAY_BUFFER, __VBO);
        char* buffer = __font->getBuffer(__text, __size);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*2*6*__text.size(), buffer, GL_STATIC_DRAW);
        glEnableVertexArrayAttrib(__VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

        glEnableVertexArrayAttrib(__VAO, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(glm::vec2)*6*__text.size()));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] buffer;
    }
    void Text::draw() const {
        if (!__font) return ;
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getFontShader());
        glBindVertexArray(__VAO);
        __font->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, __VBO);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __COLOR);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, __POSITION);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, __SCALE);
        glDrawArrays(GL_TRIANGLES, 0, __text.size()*6);
        glBindVertexArray(0);
    }
    glm::vec2 Text::getSize() const {
        return __size*__scale;
    }
}
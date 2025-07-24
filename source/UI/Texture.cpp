#include "Texture.h"
#include "General.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"

namespace MyBase {
    TextureContainer::TextureContainer(): __size(0,0), __position(0,0),__texturePosition(0,0), __textureSize(0,0) {
        glGenVertexArrays(1, &__VAO);
        glBindVertexArray(__VAO);
        
        glGenBuffers(1, &__positionCode);
        glBindBuffer(GL_UNIFORM_BUFFER, __positionCode);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__position, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__sizeCode);
        glBindBuffer(GL_UNIFORM_BUFFER, __sizeCode);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__size, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__texturePositionCode);
        glBindBuffer(GL_UNIFORM_BUFFER, __texturePositionCode);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__texturePosition, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__textureSizeCode);
        glBindBuffer(GL_UNIFORM_BUFFER, __textureSizeCode);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &__textureSize, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(0);
    }
    TextureContainer::~TextureContainer() {
        glDeleteVertexArrays(1, &__VAO);
        glDeleteBuffers(1, &__texturePositionCode);
        glDeleteBuffers(1, &__textureSizeCode);
        glDeleteBuffers(1, &__positionCode);
        glDeleteBuffers(1, &__sizeCode);
    }
    const glm::vec2& TextureContainer::getTextureExportSize() const {
        return __size;
    }
    const glm::vec2& TextureContainer::getTextureImportSize() const {
        return __textureSize;
    }
    const glm::vec2& TextureContainer::getTextureExportPosition() const {
        return __position;
    }
    const glm::vec2& TextureContainer::getTextureImportPosition() const {
        return __texturePosition;
    }
    void TextureContainer::setTextureImportPosition(const glm::vec2& texturePosition) {
        if (texturePosition != __texturePosition) {
            __texturePosition = texturePosition;
            glBindBuffer(GL_UNIFORM_BUFFER, __texturePositionCode);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__texturePosition);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }

    void TextureContainer::setTextureImportSize(const glm::vec2& textureSize) {
        if (textureSize != __textureSize) {
            __textureSize = textureSize;
            glBindBuffer(GL_UNIFORM_BUFFER, __textureSizeCode);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__textureSize);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }

    void TextureContainer::setTextureExportSize(const glm::vec2& size) {
        if (__size != size) {
            __size = size;
            glBindBuffer(GL_UNIFORM_BUFFER, __sizeCode);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__size);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            update();
        }
    }
    void TextureContainer::setTextureExportPosition(const glm::vec2& position) {
        if (__position != position) {
            __position = position;
            glBindBuffer(GL_UNIFORM_BUFFER, __positionCode);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &__position);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            update();
        }
    }
    void TextureContainer::setTexture(const GLuint& texture) {
        __texture = texture;
    }
    void TextureContainer::update() {};
    void TextureContainer::draw() const {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getImage2DShader());

        glBindVertexArray(__VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __texture);

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __positionCode);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, __sizeCode);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, __texturePositionCode);
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, __textureSizeCode);

        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getRectangle2DSet());
        glEnableVertexArrayAttrib(__VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getRectangle2DUV());
        glEnableVertexArrayAttrib(__VAO, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    }
}
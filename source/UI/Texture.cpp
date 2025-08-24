#include "Texture.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "FlyweightStorage.h"

namespace MyBase {
    Texture::Texture(const std::string & src, const bool& isReverse) {
        if (src.size()) load(src, isReverse);
    }
    glm::ivec2 Texture::getSize() const {
        return ((TextureCore*)getCore())->__size;
    }
    void Texture::load(const std::string& src, const bool& isReverse) {
        if (src.size()) {
            stbi_set_flip_vertically_on_load(isReverse);
            FlyWeightObject::load(src);
        }
    }
    Texture::~Texture() {}
    TextureCore::TextureCore() {};
    TextureCore::~TextureCore() {};

    void Texture::Bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ((TextureCore*)getCore())->__texture);
    }
    FlyWeightCore* Texture::create(const std::string& src) const {
        TextureCore* core = new TextureCore();
        int nrChannels;
        unsigned char* data = stbi_load(src.c_str(), &core->__size.x, &core->__size.y, &nrChannels, 4);
        if (!data) throw std::runtime_error("Failed to load texture: " + src);

        glGenTextures(1, &core->__texture);
        glBindTexture(GL_TEXTURE_2D, core->__texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, core->__size.x,core->__size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return core;
    }

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
    const Texture& TextureContainer::getTexture() const {
        return __texture;
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
    void TextureContainer::setTexture(const Texture& texture) {
        __texture = texture;
    }
    void TextureContainer::update() {};
    void TextureContainer::draw() const {
        if (__texture.isEmpty()) return;
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetImage2DShader());

        glBindVertexArray(__VAO);

        __texture.Bind();

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

    
    TextureViewer::TextureViewer() {}
    TextureViewer::~TextureViewer() {}
    void TextureViewer::glDraw() const {
        TextureContainer::draw();
    }
    glm::vec2 TextureViewer::getPosition() const {
        return getTextureExportPosition();
    }
    glm::vec2 TextureViewer::getSize() const {
        return getTextureExportSize();
    }
    bool TextureViewer::contains(const glm::vec2& posistion) const {
        return false;
    }
}
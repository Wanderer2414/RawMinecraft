#ifndef TEXTURE_H
#define TEXTURE_H
#include "Global.h"
namespace MyBase {
    class TextureContainer {
    public:
        TextureContainer();
        TextureContainer(const TextureContainer&) = delete;
        ~TextureContainer();
        TextureContainer& operator=(const TextureContainer&) const = delete; 

        const glm::vec2& getTextureImportSize() const;
        const glm::vec2& getTextureImportPosition() const;
        const glm::vec2& getTextureExportSize() const;
        const glm::vec2& getTextureExportPosition() const;
        void setTexture(const GLuint& texture);
        void setTextureImportPosition(const glm::vec2& texturePosition);
        void setTextureImportSize(const glm::vec2& textureSize);
        void setTextureExportSize(const glm::vec2& size);
        void setTextureExportPosition(const glm::vec2& position);
        void draw() const;
    protected:
        virtual void update();
    private:
        glm::vec2 __texturePosition, __textureSize, __size, __position;
        GLuint __VAO, __texturePositionCode, __textureSizeCode, __sizeCode, __positionCode;
        GLuint __texture;
    };
}
#endif
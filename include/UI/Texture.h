#ifndef TEXTURE_H
#define TEXTURE_H
#include "Controller2D.h"
#include "FlyweightStorage.h"
#include "Global.h"
namespace MyBase {
    class TextureCore;
    class Texture: private FlyWeightObject {
    public:
        Texture(const std::string& src = "", const bool& isReverse = true);
        ~Texture();
        glm::ivec2 getSize() const;
        void load(const std::string& src, const bool& isReverse = true);
        void Bind() const;
        using FlyWeightObject::getSource;
        using FlyWeightObject::isEmpty;
    protected:
    private:
        FlyWeightCore* create(const std::string& src) const override;
    };
    class TextureCore: public FlyWeightCore {
    public:
        TextureCore(const TextureCore&) =delete;
        TextureCore(TextureCore&&) = delete;
        friend class Texture;
    private:
        ~TextureCore();
        TextureCore();
        GLuint __texture;
        glm::ivec2 __size;
    };
    class TextureContainer {
    public:
        TextureContainer();
        TextureContainer(const TextureContainer&) = delete;
        ~TextureContainer();
        TextureContainer& operator=(const TextureContainer&) const = delete; 

        const Texture& getTexture() const;
        const glm::vec2& getTextureImportSize() const;
        const glm::vec2& getTextureImportPosition() const;
        const glm::vec2& getTextureExportSize() const;
        const glm::vec2& getTextureExportPosition() const;
        void setTexture(const Texture& texture);
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
        Texture __texture;
    };
    class TextureViewer: private TextureContainer, public Controller2D {
    public:
        TextureViewer();
        TextureViewer(const TextureViewer&) = delete;
        ~TextureViewer();
        TextureViewer& operator=(const TextureViewer&) const = delete; 

        using TextureContainer::getTextureImportSize;
        using TextureContainer::getTextureImportPosition;
        
        using TextureContainer::setTextureImportPosition;
        using TextureContainer::setTextureImportSize;
        using TextureContainer::setTextureExportSize;
        using TextureContainer::setTextureExportPosition;
        using TextureContainer::setTexture;
        virtual glm::vec2       getPosition() const override,
                                getSize() const override;
    protected:
    private:
        virtual void glDraw() const override;
        virtual bool contains(const glm::vec2& posistion) const override;
    };
}
#endif
#ifndef BLOCK_H
#define BLOCK_H
#include "Controller3D.h"
#include "Ray.h"
#include "Rect3D.h"

class BlockCatogary {
public:
    enum Catogary {
        Air, Dirt
    };
    BlockCatogary();
    ~BlockCatogary();

    GLuint getBlock(const int& index) const;
    GLuint BlockTexture;
    static BlockCatogary* Default;
private:
    vector<GLuint>      pPtr;
};

class Block: public Controller3D {
public:
    Block();
    ~Block();
    BlockCatogary::Catogary type;
    virtual bool        setHover(const Ray3f& hover) override;
    virtual void        setPosition(const float& x, const float& y, const float& z),
                        setPosition(const glm::vec3& position);
                        
    virtual bool        contains(const Ray3f& point)       const override;
protected:
    virtual void        draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    virtual void        glDraw() const override;
private:
    glm::vec3           pPosition;
};
#endif
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

class Block {
public:
    Block();
    ~Block();
    BlockCatogary::Catogary type;
    virtual void        setPosition(const float& x, const float& y, const float& z),
                        setPosition(const glm::vec3& position);
                        
    virtual bool        contains(const Ray3f& point)       const;
protected:
    virtual void        glDraw() const;
private:
    glm::vec3           pPosition;
};
#endif
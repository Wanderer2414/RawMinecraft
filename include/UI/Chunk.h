#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Controller3D.h"
class Chunk: public Controller3D {
public:
    Chunk();
    ~Chunk();

    virtual void    setPosition(const int& x, const int& y, const int& z),
                    setPosition(const glm::vec3& position);
    
    BlockCatogary::Catogary& getBlocks(const int& x, const int& y, const int& z);
protected:
    virtual void    glDraw() const override;
private:
    glm::vec3                    pPosition;
};
#endif
#ifndef CHUNK_H
#define CHUNK_H
#include "Controller3D.h"
class Chunk: public Controller3D {
public:
    Chunk();
    ~Chunk();

    virtual void    setPosition(const int& x, const int& y, const int& z),
                    setPosition(const Vector3i& position);
protected:
    virtual void    glDraw() const override;
private:
    Vector3i        pPosition;
};
#endif
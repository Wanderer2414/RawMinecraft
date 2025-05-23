#include "Chunk.h"
#include "Block.h"
#include <GL/gl.h>

Chunk::Chunk() {

}
Chunk::~Chunk() {

}

void Chunk::setPosition(const int& x, const int& y, const int& z) {
    pPosition = {x, y, z};
}
void Chunk::setPosition(const Vector3i& position) {
    pPosition = position;
}

void Chunk::glDraw() const {
    glLineWidth(3.0);
    Vector3i vecs[] = {
        {pPosition.x, pPosition.y, pPosition.z},
        {pPosition.x, pPosition.y+16, pPosition.z},
        {pPosition.x+16, pPosition.y+16, pPosition.z},
        {pPosition.x+16, pPosition.y, pPosition.z},
    };

    // glBegin(GL_LINE_STRIP);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    //     glVertex3i(pPosition.x, pPosition.y+16, pPosition.z+16);
    //     glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z+16);
    //     glVertex3i(pPosition.x+16, pPosition.y, pPosition.z+16);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    // glEnd();

    // glBegin(GL_LINES);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    //     glVertex3i(pPosition.x, pPosition.y+16, pPosition.z);
    //     glVertex3i(pPosition.x, pPosition.y+16, pPosition.z+16);
    //     glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z);
    //     glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z+16);
    //     glVertex3i(pPosition.x+16, pPosition.y, pPosition.z);
    //     glVertex3i(pPosition.x+16, pPosition.y, pPosition.z+16);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z);
    //     glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    // glEnd();
        
}

// BlockCatogary::Catogary& Chunk::getBlocks(const int& x, const int& y, const int& z) {
//     return pBlocks[x][y][z];
// }
#include "Chunk.h"
#include "Block.h"

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
    glColor3f(1, 0, 0);
    glLineWidth(3.0);
    glBegin(GL_LINE_STRIP);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z);
        glVertex3i(pPosition.x, pPosition.y+16, pPosition.z);
        glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z);
        glVertex3i(pPosition.x+16, pPosition.y, pPosition.z);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
        glVertex3i(pPosition.x, pPosition.y+16, pPosition.z+16);
        glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z+16);
        glVertex3i(pPosition.x+16, pPosition.y, pPosition.z+16);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    glEnd();

    glBegin(GL_LINES);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
        glVertex3i(pPosition.x, pPosition.y+16, pPosition.z);
        glVertex3i(pPosition.x, pPosition.y+16, pPosition.z+16);
        glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z);
        glVertex3i(pPosition.x+16, pPosition.y+16, pPosition.z+16);
        glVertex3i(pPosition.x+16, pPosition.y, pPosition.z);
        glVertex3i(pPosition.x+16, pPosition.y, pPosition.z+16);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z);
        glVertex3i(pPosition.x, pPosition.y, pPosition.z+16);
    glEnd();
        
}

BlockCatogary::Catogary& Chunk::getBlocks(const int& x, const int& y, const int& z) {
    return pBlocks[x][y][z];
}
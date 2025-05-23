#include "Block.h"
#include "General.h"
#include <GL/gl.h>

BlockCatogary::BlockCatogary() {
    pPtr.resize(2, 0);
    pStorage.resize(2, 0);
    for (int i = 0; i<2; i++) {
        pStorage[i] = new Image();
    }
    pPtr[0] = 0;
    pStorage[0] = 0;
    pStorage[1]->loadFromFile("assets/images/Dirt.png");
    glGenTextures(1, &pPtr[1]);
    glBindTexture(GL_TEXTURE_2D, pPtr[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pStorage[1]->getSize().x,pStorage[1]->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pStorage[1]->getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
BlockCatogary::~BlockCatogary() {
    for (int i =0; i<pStorage.size(); i++) {
        if (pStorage[i]) glDeleteTextures(1, &pPtr[i]);
    }
    glDeleteTextures(1, &pPtr[1]);
}

uint BlockCatogary::operator[](const Catogary& type) const {
    return pPtr[type];
}

Block::Block(BlockCatogary* block_catorgary): pBlockCatogary(block_catorgary) {
    pPosition = {0,0,0};


    pPlane[0] = {pPosition, pPosition+Vector3f(0, 1, 0), pPosition+Vector3f(1, 1, 0)};
    pPlane[5] = {pPosition+Vector3f(1, 1, 1), pPosition+Vector3f(0, 1, 1), pPosition+Vector3f(0, 0, 1)};

    pPlane[1] = {pPosition+Vector3f(0, 0, 1),pPosition,  pPosition+Vector3f(1, 0, 0)};
    pPlane[4] = { pPosition+Vector3f(1, 1, 1), pPosition+Vector3f(1, 1, 0), pPosition+Vector3f(0,1 ,0 )};

    pPlane[2] = {pPosition+Vector3f(0, 1, 1), pPosition + Vector3f(0, 1, 0), pPosition};
    pPlane[3] = {  pPosition+Vector3f(1, 0, 1), pPosition+Vector3f(1, 0, 0), pPosition+Vector3f(1, 1, 0)};
}
Block::~Block() {
}
bool Block::setHover(const Ray3f& sight) {
    bool hover = false;
    Vector3f ray = sight;
    if (ray.z > 0) hover = pPlane[0].isIntersect(sight) || hover;
    else if (ray.z < 0) hover = pPlane[5].isIntersect(sight) || hover;

    if (ray.x > 0) hover = pPlane[2].isIntersect(sight) || hover;
    else if (ray.x < 0) hover = pPlane[3].isIntersect(sight) || hover;
    
    if (ray.y > 0) hover = pPlane[1].isIntersect(sight) || hover;
    else if (ray.y < 0) hover = pPlane[4].isIntersect(sight) || hover;
    return Controller3D::setHover(hover);
}
void Block::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Block::setPosition(const Vector3f& position) {
    Vector3f delta = position - pPosition;
    for (int i = 0; i<6; i++) pPlane[i].move(delta);
    setPosition(position.x, position.y, position.z);
}
bool Block::contains(const Ray3f& point) const  {
    return false;
}
void Block::draw(RenderTarget& target, RenderStates state) const {
}
void Block::glDraw() const {
    // if (isFocus()) glColor3f(1, 0, 0);
    // else glColor3f(1, 1, 1);
    // glColor3f(1, 1, 1);
    // glBindTexture(GL_TEXTURE_2D, (*pBlockCatogary)[type]);
    // for (int i = 0; i<6; i++) {
    //     glBegin(GL_QUADS);
    //         glTexCoord2f(1.f/3, 0);
    //         glVertex3f(pPlane[i][0]);
    //         glTexCoord2f(1.f/3, 1.f/3);
    //         glVertex3f(pPlane[i][1]);
    //         glTexCoord2f(2.f/3, 1.f/3);
    //         glVertex3f(pPlane[i][2]);
    //         glTexCoord2f(2.f/3, 0);
    //         glVertex3f(pPlane[i][3]);
    //     glEnd();
    // }
    // if (isHovered()) {
    //     glColor3f(0.1,0.1,0.1);
    //     glLineWidth(2);
    //     for (int i = 0; i<6; i++) {
    //         glBegin(GL_LINE_STRIP);
    //             glVertex3f(pPlane[i][0]);
    //             glVertex3f(pPlane[i][1]);
    //             glVertex3f(pPlane[i][2]);
    //             glVertex3f(pPlane[i][3]);
    //             glVertex3f(pPlane[i][0]);
    //         glEnd();
    //     }
    // }
}
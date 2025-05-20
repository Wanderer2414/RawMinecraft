#include "Cube.h"
#include "General.h"
#include <GL/gl.h>

Cube::Cube() {
    pPosition = {0,0,0};
    pImage.loadFromFile("assets/images/Dirt.png");
    glGenTextures(1, &pTextureId);
    glBindTexture(GL_TEXTURE_2D, pTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage.getSize().x,pImage.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    pPlane[0] = {pPosition, pPosition+Vector3f(0, 1, 0), pPosition+Vector3f(1, 1, 0)};
    pPlane[5] = {pPosition+Vector3f(1, 1, 1), pPosition+Vector3f(0, 1, 1), pPosition+Vector3f(0, 0, 1)};

    pPlane[1] = {pPosition+Vector3f(0, 0, 1),pPosition,  pPosition+Vector3f(1, 0, 0)};
    pPlane[4] = { pPosition+Vector3f(1, 1, 1), pPosition+Vector3f(1, 1, 0), pPosition+Vector3f(0,1 ,0 )};

    pPlane[2] = {pPosition+Vector3f(0, 1, 1), pPosition + Vector3f(0, 1, 0), pPosition};
    pPlane[3] = {  pPosition+Vector3f(1, 0, 1), pPosition+Vector3f(1, 0, 0), pPosition+Vector3f(1, 1, 0)};
}
Cube::~Cube() {
    glDeleteTextures(1, &pTextureId);
}
bool Cube::setHover(const Ray3f& sight) {
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
void Cube::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Cube::setPosition(const Vector3f& position) {
    Vector3f delta = position - pPosition;
    for (int i = 0; i<6; i++) pPlane[i].move(delta);
    setPosition(position.x, position.y, position.z);
}
bool Cube::contains(const Ray3f& point) const  {
    return false;
}
void Cube::draw(RenderTarget& target, RenderStates state) const {
}
void Cube::glDraw() const {
    if (isFocus()) glColor3f(1, 0, 0);
    else glColor3f(1, 1, 1);
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, pTextureId);
    for (int i = 0; i<6; i++) {
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);
            glVertex3f(pPlane[i][0]);
            glTexCoord2f(0, 1);
            glVertex3f(pPlane[i][1]);
            glTexCoord2f(1, 1);
            glVertex3f(pPlane[i][2]);
            glTexCoord2f(1, 0);
            glVertex3f(pPlane[i][3]);
        glEnd();
    }
    if (isHovered()) {
        glColor3f(0.1,0.1,0.1);
        glLineWidth(2);
        for (int i = 0; i<6; i++) {
            glBegin(GL_LINE_STRIP);
                glVertex3f(pPlane[i][0]);
                glVertex3f(pPlane[i][1]);
                glVertex3f(pPlane[i][2]);
                glVertex3f(pPlane[i][3]);
                glVertex3f(pPlane[i][0]);
            glEnd();
        }
    }
}
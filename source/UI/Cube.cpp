#include "Cube.h"
#include "General.h"
#include <GL/gl.h>

Cube::Cube() {
    pPosition = {0,0,0};
    pOutlineThickness = 1;
    pFillColor = Color::Transparent;
    pOutlineColor = Color::White;
}
Cube::~Cube() {

}
bool Cube::setHover(const Ray3f& sight) {
    bool hover = false;
    for (int i = 0; i<6; i++) 
        if (pPlane[i].isIntersect(sight)) {
            return Controller3D::setHover(true);
        }
    return Controller3D::setHover(false);
}
void Cube::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Cube::setPosition(const Vector3f& position) {
    Vector3f delta = position - pPosition;
    for (int i = 0; i<6; i++) pPlane[i].move(delta);
    setPosition(position.x, position.y, position.z);
}
void Cube::setSize(const float& side) {
    pPlane[0] = {pPosition, pPosition+Vector3f(0, side, 0), pPosition+Vector3f(side, side, 0)};
    pPlane[5] = {pPosition+Vector3f(side, side, side), pPosition+Vector3f(0, side, side), pPosition+Vector3f(0, 0, side)};

    pPlane[1] = {pPosition+Vector3f(side, 0, side),pPosition+Vector3f(0,0 , side),  pPosition};
    pPlane[4] = { pPosition+Vector3f(0, side, 0), pPosition+Vector3f(0,side , side), pPosition+Vector3f(side, side, side)};

    pPlane[2] = {pPosition, pPosition+Vector3f(0, 0, side), pPosition+Vector3f(0, side, side)};
    pPlane[3] = {pPosition+Vector3f(side, side, side),  pPosition+Vector3f(side, 0, side), pPosition+Vector3f(side, 0, 0)};
}
void Cube::setOutlineThickness(const float& thickness) {
    pOutlineThickness = thickness;
}
bool Cube::contains(const Ray3f& point) const  {
    return false;
}
void Cube::setFillColor(const Color& color) {
    pFillColor = color;
}
void Cube::setOutlineColor(const Color& color) {
    pOutlineColor = color;
}
void Cube::draw(RenderTarget& target, RenderStates state) const {
    target.popGLStates();
    glLineWidth(pOutlineThickness);
    glColor3f(pOutlineColor.r, pOutlineColor.g, pOutlineColor.b);
    if (isHovered()) glColor3f(1, 0, 0);
    else glColor3f(1, 1, 1);
    for (int i = 0; i<6; i++) {
        glBegin(GL_LINE_STRIP);
            for (int j = -1; j<4; j++) glVertex3f(pPlane[i][j]);
        glEnd();
    }
    target.pushGLStates();
}
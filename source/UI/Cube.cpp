#include "Cube.h"
#include "SFML/System/Vector2.hpp"
#include <GL/gl.h>

Cube::Cube() {
    pSide = 0;
    pPosition = {0,0,0};
    pOutlineThickness = 1;
    pFillColor = Color::Transparent;
    pOutlineColor = Color::White;
}
Cube::~Cube() {

}
void Cube::setPosition(const float& x, const float& y, const float& z) {
    pPosition = {x, y, z};
}
void Cube::setPosition(const Vector3f& position) {
    setPosition(position.x, position.y, position.z);
}
void Cube::setSize(const float& side) {
    pSide = side;
}
void Cube::setOutlineThickness(const float& thickness) {
    pOutlineThickness = thickness;
}
bool Cube::contains(const Vector2f& point) const  {
    return false;
}
Vector2f Cube::getPosition() const {
    return {0, 0};
}
Vector2f Cube::getCenter() const {
    return {0,0 };
}
Vector2f Cube::getSize() const {
    return {0,0};
}
void Cube::update(){
    
}
void Cube::setPosition(const float& x, const float& y) {

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
    glBegin(GL_LINE_STRIP);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z);
        glVertex3f(pPosition.x, pPosition.y+pSide, pPosition.z);
        glVertex3f(pPosition.x + pSide, pPosition.y+pSide, pPosition.z);
        glVertex3f(pPosition.x + pSide, pPosition.y, pPosition.z);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z);
    glEnd();
    glBegin(GL_LINE_STRIP);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z + pSide);
        glVertex3f(pPosition.x, pPosition.y+pSide, pPosition.z + pSide);
        glVertex3f(pPosition.x+ pSide, pPosition.y+pSide, pPosition.z + pSide);
        glVertex3f(pPosition.x + pSide, pPosition.y, pPosition.z + pSide);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z + pSide);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z + pSide);

        glVertex3f(pPosition.x, pPosition.y+pSide, pPosition.z);
        glVertex3f(pPosition.x, pPosition.y+pSide, pPosition.z + pSide);

        glVertex3f(pPosition.x + pSide, pPosition.y+pSide, pPosition.z);
        glVertex3f(pPosition.x+ pSide, pPosition.y+pSide, pPosition.z + pSide);

        glVertex3f(pPosition.x + pSide, pPosition.y, pPosition.z);
        glVertex3f(pPosition.x + pSide, pPosition.y, pPosition.z + pSide);

        glVertex3f(pPosition.x, pPosition.y, pPosition.z);
        glVertex3f(pPosition.x, pPosition.y, pPosition.z + pSide);
    glEnd();
    target.pushGLStates();
}
#include "Coordinate.h"
#include <GL/gl.h>

Coordinate::Coordinate() {

}
Coordinate::~Coordinate() {

}

void Coordinate::draw(RenderTarget& target, RenderStates state) const {
}
void Coordinate::glDraw() const {
    glBegin(GL_LINES);
        for (int i = -5; i<5; i++) {
            glColor3f(1, 0, 0);
            glVertex3f(-10, i,0); glVertex3f(10, i, 0);
            glColor3f(0, 1, 0);
            glVertex3f(i, -10,0); glVertex3f(i, 10, 0);
        }
        glColor3f(0, 0, 1);glVertex3f(0, 0,-10); glVertex3f(0, 0, 10);
    glEnd();
}
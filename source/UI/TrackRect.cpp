#include "TrackRect.h"
#include "Controller3D.h"
#include "General.h"
namespace MyBase3D {

    TrackRect::TrackRect() {

    }
    TrackRect::~TrackRect() {
    
    }
    bool TrackRect::setHover(const Ray3f& sight) {
        bool hover = false;
        __position = __rect.getIntersect(sight);
        return Controller3D::setHover(hover);
    }
    void TrackRect::setPosition(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
        __rect = {a,b,c};
    }
    
    void TrackRect::draw(sf::RenderTarget& target, sf::RenderStates state) const {
        
        // glm::vec3 AB = (pRect[1]-pRect[0])/10.f, BC = (pRect[2]-pRect[1])/10.f;
        // if (pRect.contains(position)) glColor3f(0, 0, 1);
        // else glColor3f(1, 1, 1);
    
        // glBegin(GL_LINES);
        // for (float i = 0; i<11; i++) {
        //     glVertex3f(pRect[0]+AB*i);
        //     glVertex3f(pRect[3]+AB*i);
    
        //     glVertex3f(pRect[0]+BC*i);
        //     glVertex3f(pRect[1]+BC*i);
        // }    
        // glVertex3f(pRect[0]);
        // glVertex3f(pRect[1]);
        // glVertex3f(pRect[3]);
        // glVertex3f(pRect[2]);
        // glEnd();
    
        // glPointSize(10);
        // glBegin(GL_LINES);
        //     glVertex3f(position-AB);
        //     glVertex3f(position+AB);
    
        //     glVertex3f(position-BC);
        //     glVertex3f(position+BC);
        // glEnd();
    }
}
#ifndef TRACK_RECT_H
#define TRACK_RECT_H
#include "Controller3D.h"
#include "Ray.h"
#include "Rect3D.h"
namespace MyBase3D {

class TrackRect: public Controller3D {
    public:
        TrackRect();
        ~TrackRect();
    
        virtual bool setHover(const Ray3f& sight) override;
    
        virtual void setPosition(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
    private:
        Rect3f      __rect;
        glm::vec3   __position;
    };
}
#endif
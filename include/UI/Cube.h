#ifndef CUBE_H
#define CUBE_H
#include "BaseShape.h"
#include "Camera.h"
#include "Ray.h"
#include "Rect3D.h"

class Cube: public Drawable {
public:
    Cube();
    ~Cube();
    virtual void        setPosition(const float& x, const float& y, const float& z),
                        setPosition(const Vector3f& position),
                        setSize(const float& side),

                        setPosition(const float& x, const float& y),
                        
                        setFillColor(const Color& color),
                        
                        setOutlineThickness(const float& thickness),
                        setOutlineColor(const Color& color);
                        
    virtual bool        contains(const Ray3f& point)       const;
    virtual Vector2f    getPosition()                       const;
protected:
    virtual void        draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    float               pOutlineThickness;
    Vector3f            pPosition;
    Color               pFillColor, pOutlineColor;
    Rect3f              pPlane[6];
};
#endif
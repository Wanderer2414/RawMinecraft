#ifndef CUBE_H
#define CUBE_H
#include "BaseShape.h"
#include "Camera.h"
#include "Controller3D.h"
#include "Ray.h"
#include "Rect3D.h"

class Cube: public Controller3D {
public:
    Cube();
    ~Cube();
    virtual bool        setHover(const Ray3f& hover) override;
    virtual void        setPosition(const float& x, const float& y, const float& z),
                        setPosition(const Vector3f& position),
                        setSize(const float& side),

                        setFillColor(const Color& color),
                        
                        setOutlineThickness(const float& thickness),
                        setOutlineColor(const Color& color);
                        
    virtual bool        contains(const Ray3f& point)       const override;
protected:
    virtual void        draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    virtual void        glDraw() const override;
private:
    Image               pImage;
    GLuint              pTextureId;
    float               pOutlineThickness;
    Vector3f            pPosition;
    Color               pFillColor, pOutlineColor;
    Rect3f              pPlane[6];
};
#endif
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
                        setPosition(const Vector3f& position);
                        
    virtual bool        contains(const Ray3f& point)       const override;
protected:
    virtual void        draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    virtual void        glDraw() const override;
private:
    Image               pImage;
    GLuint              pTextureId;
    Vector3f            pPosition;
    Rect3f              pPlane[6];
};
#endif
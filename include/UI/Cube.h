#ifndef CUBE_H
#define CUBE_H
#include "BaseShape.h"

class Cube: public BaseShape {
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
                        
    virtual bool        contains(const Vector2f& point)     const override;
    virtual Vector2f    getPosition()                       const,
                        getCenter()                         const override,
                        getSize()                           const override;
    virtual void        update()                                  override;
protected:
    virtual void        draw(RenderTarget& target, RenderStates state = RenderStates::Default) const;
private:
    float               pSide, pOutlineThickness;
    Vector3f            pPosition;
    Color               pFillColor, pOutlineColor;
};
#endif
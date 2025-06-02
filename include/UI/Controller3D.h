#ifndef Controller_3D_H
#define Controller_3D_H
#include "Global.h"
#include "Ray.h"
namespace MyBase3D {

class Container3D;

class Controller3D {
public:
    Controller3D();
    ~Controller3D();
    virtual bool    isHovered() const,
                    isReleased() const,
                    isDoubleClick() const,
                    isPressed() const,
                    isFocus()   const,
                    setHover(const bool& hover),
                    setHover(const Ray3f& sight);

    virtual void    setFocus(const bool& focus),
                    reset(),
                    update();      

    friend Container3D;
protected:
    virtual bool    handle(GLFWwindow* window);
    virtual bool    contains(const Ray3f& position) const;
    virtual void    glDraw() const;

private:
    unsigned int    __doubleClickCount;
    bool            __isHovered,
                    __isPressed,
                    __isDoubleClick,
                    __isReleased,
                    __isFocus;
};
}
#endif
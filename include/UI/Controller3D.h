#ifndef Controller_3D_H
#define Controller_3D_H
#include "Global.h"
#include "Ray.h"

class Container3D;

class Controller3D: public Drawable {
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
    virtual         catch_function(CatchEvent);
    virtual         catch_function(BeforeCatch);
    virtual         catch_function(AfterCatch);
    virtual         handle_function(handle);
    virtual bool    contains(const Ray3f& position) const;
    virtual void    draw(RenderTarget& target, RenderStates state) const override;

private:
    unsigned int    double_click_count;
    bool            pIsHovered,
                    pIsPressed,
                    pIsDoubleClick,
                    pIsReleased,
                    pIsFocus;
};
#endif
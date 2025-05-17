#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Global.h"

class Container;

class Controller: public Drawable {
public:
    Controller();
    ~Controller();
    virtual bool    isHovered() const,
                    isReleased() const,
                    isDoubleClick() const,
                    isPressed() const,
                    isFocus()   const,
                    setHover(const bool& hover),
                    setHover(const Vector2f& position);

    virtual Vector2f getPosition() const,
                    getSize() const;
    virtual void    setPosition(const float& x, const float& y),
                    setFocus(const bool& focus),
                    reset(),
                    update();      

    friend Container;
protected:
    virtual         catch_function(CatchEvent);
    virtual         catch_function(BeforeCatch);
    virtual         catch_function(AfterCatch);
    virtual         handle_function(handle);
    virtual bool    contains(const Vector2f& position) const;
    virtual void    draw(RenderTarget& target, RenderStates state) const override;

private:
    unsigned int    double_click_count;
    bool            m_is_hovered,
                    m_is_pressed,
                    m_is_double_click,
                    m_is_released,
                    m_is_focus;
};
#endif
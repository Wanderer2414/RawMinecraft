#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Global.h"
namespace MyBase {
    class Container;

    class Controller {
    public:
        Controller();
        ~Controller();
        virtual bool            isHovered()         const,
                                isReleased()        const,
                                isDoubleClick()     const,
                                isPressed()         const,
                                isFocus()           const,
                                setHover(const bool& hover),
                                setHover(const glm::vec2& position);

        virtual glm::vec2       getPosition() const,
                                getSize() const;
        virtual void            setPosition(const float& x, const float& y),
                                setFocus(const bool& focus),
                                reset(),
                                update();      

        friend Container;
        virtual void    glDraw() const;
    protected:
        virtual bool    handle(GLFWwindow* window);
        virtual bool    contains(const glm::vec2& position) const;

    private:
        unsigned int    __doubleClickCount;
        bool            __isHovered,
                        __isPressed,
                        __isDoubleClick,
                        __isReleased,
                        __isFocus;
    };
};
#endif
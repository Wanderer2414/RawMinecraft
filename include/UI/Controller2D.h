#ifndef CONTROLLER_2D_H
#define CONTROLLER_2D_H
#include "Controller.h"
namespace MyBase {
    class Controller2D: public Controller {
    public:
       Controller2D();
       ~Controller2D();

        virtual bool            isHovered()         const,
                                isReleased()        const,
                                isDoubleClick()     const,
                                isPressed()         const,
                                isMouseDown()       const,
                                isFocus()           const,
                                isVisible()         const,
                                setHover(const bool& hover),
                                setHover(const glm::vec2& position);
        virtual glm::vec2       getPosition() const = 0,
                                getSize() const = 0;
        virtual void            setFocus(const bool& focus),
                                setVisible(const bool& visible),
                                reset();
        friend class Container2D;
    protected:
        virtual bool    contains(const glm::vec2& position) const = 0;
        virtual bool    catchEvent(GLFWwindow* window) override;
        virtual bool    __lostFocus(), __getFocus(), __onFocus(),
                        __clicked(), __mouseDown(), __mouseRelease(), 
                        __hover(), __lostHover();
    private:
        bool            __isVisible;
        unsigned int    __clickCount;
        bool            __isHovered,
                        __isPressed, __isMouseDown,
                        __isDoubleClick,
                        __isReleased,
                        __isFocus;
    };
}
#endif
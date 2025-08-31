#ifndef WRAPPER_H
#define WRAPPER_H
#include "Controller2D.h"
namespace MyBase {
    class Wrapper: public MyBase::Controller2D {
    public:
        Wrapper(Controller2D* controller);
        ~Wrapper();
        Wrapper(const Wrapper&) = delete;
        Wrapper& operator=(const Wrapper&) const = delete; 
        void changeState(Controller2D* controller);
        bool                    isEmpty() const;
        virtual bool            isHovered()         const   override,
                                isReleased()        const   override,
                                isDoubleClick()     const   override,
                                isPressed()         const   override,
                                isMouseDown()       const   override,
                                isFocus()           const   override,
                                isVisible()         const   override,
                                setHover(const bool& hover) override,
                                setHover(const glm::vec2& position) override;
        virtual glm::vec2       getPosition() const override,
                                getSize() const     override;
        virtual void            setFocus(const bool& focus)     override,
                                setVisible(const bool& visible) override,
                                reset()                         override,
                                update()                        override;
        Controller2D            *getState();
    protected:
        virtual bool            contains(const glm::vec2& position) const   override;
        virtual bool            catchEvent(GLFWwindow* window)              override;
        virtual bool            handle(GLFWwindow* window)                  override;
        virtual void            glDraw() const                              override;
        virtual void            glDrawTransparent() const                   override;
    protected:
    private:
        Controller2D* __controller;
    };
}
#endif
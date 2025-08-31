#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Clock.h"
#include "Controller2D.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "Shape.h"
#include "Text.h"
namespace MyBase {
    class Textbox: public Text, public ShapeContainer, public Controller2D {
    public:
        Textbox();
        Textbox(const Textbox&) = delete;
        ~Textbox();
        Textbox& operator=(const Textbox&) const = delete;

        virtual glm::vec2       getPosition() const override,
                                getSize() const override; 
        void                    setSize(const glm::vec2& size, const float& roundness);
        void                    setScale(const glm::vec2& scale);
        void                    setPosition(const glm::vec2& position);
        void                    update() override;
        void                    setText(const std::string& text);
    protected:
        virtual bool            __focus(GLFWwindow*) override, __lostFocus(GLFWwindow*) override, __onFocus(GLFWwindow*) override;
    private:
        bool                    __isCursorShow;
        float                   __roundness;
        Clock                   __cursorClock;
        glm::vec2               __size;
        RoundedRectangle        __bound;
        Rectangle               __textCursor;
        ShapeContainer          __textCursorContainer;
        using ShapeContainer::setMarginWidth;

        bool                    contains(const glm::vec2& position) const override;
        void                    glDraw() const override;
    };
}
#endif
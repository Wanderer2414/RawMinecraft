#ifndef BUTTON_H
#define BUTTON_H
#include "Controller2D.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "Text.h"

namespace MyBase  {
    template<typename T>
    class Button: public Controller2D, public ShapeContainer, public Text {
    public:
        Button();
        ~Button();
        glm::vec2 getSize() const override;
        glm::vec2 getPosition() const override;
        void setPosition(const glm::vec2& position);
        void move(const glm::vec2& offset);
        void setFont(const Font& font);
        void setNormalColor(const Color& color);
        void setHoverColor(const Color& color);
        void setClickColor(const Color& color);
        void setText(const std::string& text);
        void setScale(const glm::vec2& scale);
    protected:
        void update() override;
        virtual bool    __mouseDown(GLFWwindow*) override,
                        __hover() override,
                        __lostHover() override,
                        __mouseRelease(GLFWwindow*) override;
    private:
        virtual T&          getShape() = 0;
        virtual const T&    getShape() const = 0;
        virtual void        glDraw() const override;
        bool                contains(const glm::vec2& position) const override;
        Color   __hoverColor, __clickColor, __normalColor;
    };
    template class Button<Ellipse>;
    template class Button<Rectangle>;
    template class Button<RoundedRectangle>;
    class RectangleButton: public Button<Rectangle> {
    public:
        RectangleButton();
        ~RectangleButton();
        void setSize(const glm::vec2& size);
    private:
        glm::vec2 __size;
        Rectangle& getShape() override;
        const Rectangle& getShape() const override;
        Rectangle __shape;
    };

    class RoundedRectangleButton: public Button<RoundedRectangle> {
    public:
        RoundedRectangleButton();
        ~RoundedRectangleButton();
        void setSize(const glm::vec2& size, const float& radius);
    private:
        float __radius;
        glm::vec2 __size;
        RoundedRectangle& getShape() override;
        const RoundedRectangle& getShape() const override;
        RoundedRectangle __shape;
    };

    class EllipseButton: public Button<Ellipse> {
    public:
        EllipseButton();
        ~EllipseButton();
        void setSize(const glm::vec2& size);
    private:
        glm::vec2 __size;
        Ellipse& getShape() override;
        const Ellipse& getShape() const override;
        Ellipse __shape;
    };
}
#endif
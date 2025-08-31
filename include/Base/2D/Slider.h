#ifndef SLIDER_H
#define SLIDER_H

#include "Container2D.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
#include "Text.h"
#include "Texture.h"
#include "Button.h"

namespace MyBase  {
    class Slider : public Container2D, public ShapeContainer, public Text{
    public:
        Slider();
        ~Slider();
        
        void setPosition(const glm::vec2& position);
        void setWidth(const float& width);
        void setMaxValue(const int & value);
        void setMinValue(const int & value);
        void setText(const std::string& text);

        glm::vec2 getPosition() const override;
        glm::vec2 getSize() const override;
        int getValue() const;

    private:
        const double height = 0.1f;
        bool            isScroll;
        float           width;
        int             value, minVal, maxVal;
        Rectangle       __rectangle;
        RectangleButton __scrollButton;
        float           mouseStartDrag;
        bool            catchEvent(GLFWwindow * window) override;
        void            glDraw() const override;
        bool            contains(const glm::vec2& position) const override;
        void            update() override;
    };
}

#endif

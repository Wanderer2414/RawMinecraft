#ifndef VERTICAL_BAR_H
#define VERTICAL_BAR_H
#include "Button.h"
#include "Container2D.h"
#include "Global.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyBase {
    class VerticalBar: public Container2D, public ShapeContainer {
    public:
       VerticalBar();
       ~VerticalBar();
        static constexpr float width = 0.02;
        float getValue() const;
        void setPosition(const glm::vec2& position);
        void setHeight(const float& height);
        void setMaxValue(const float& value);
        
        glm::vec2   getPosition()   const override, 
                    getSize()       const override;
    protected:
    private:
        bool            __isScrollDown;
        float           __mouseStartDrag;
        float           __height, __value, __maxValues;
        Rectangle       __rectangle;
        RectangleButton __scrollButton;
        bool            catchEvent(GLFWwindow* window) override;
        void            glDraw() const override;
        bool            contains(const glm::vec2& position) const override;
        void            update() override;
    };
}
#endif
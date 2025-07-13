#ifndef LIST_H
#define LIST_H
#include "Button.h"
#include "Container2D.h"
#include "RoundedRectangle.h"
#include "Shape.h"
#include "VerticalBar.h"
namespace MyBase {
    class List: public Container2D, public ShapeContainer, public Text {
    public:
        List();
        ~List();
        bool contains(const glm::vec2& position) const override;
        using Container2D::setHover;
        bool setHover(const glm::vec2& position) override;
        void setSize(const glm::vec2& size);
        void setList(const std::vector<std::string>& list);
        void setSubScale(const glm::vec2& scale);
        void setNormalColor(const Color& color);
        void setHoverColor(const Color& color);
        void setClickColor(const Color& color);
        void setFont(const Font& font);
        void clear() override;
        void setPosition(const glm::vec2& position);
        glm::vec2 getSize() const override, getPosition() const override;
    private:
        float       __deltaY, __childrenHeight;
        glm::vec2   __size, __subScale, __subSize, __subPadding;
        Color       __normalColor, __hoverColor, __clickColor;
        std::vector<RoundedRectangleButton*> __buttons;
        RoundedRectangle __rectangle;
        VerticalBar     __verticalBar;
        bool catchEvent(GLFWwindow* window) override;
        void glDraw() const override;
        void __moveChild(const glm::vec2& offset);
    };
}
#endif
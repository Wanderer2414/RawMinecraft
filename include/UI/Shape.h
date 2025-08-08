#ifndef SHAPE_H
#define SHAPE_H
#include "Global.h"
#include "Color.h"
namespace MyBase {
    class Shape {
    public:
        void draw() const;
    protected:
        Shape();
        ~Shape();
        virtual bool contains(const glm::vec2& position) const = 0;
        virtual size_t getPointCount() const = 0;
        virtual glm::vec2 getPoint(const size_t& index) const = 0;
        virtual glm::vec2 getCenter() const = 0;
        friend class ShapeContainer;
        friend class ShapeManager;
    protected:
        void update();
    private:
        GLuint __VBO, __VAO;
    };
    class ShapeContainer {
    public:
        ShapeContainer();
        ~ShapeContainer();
        Color getColor() const;
        glm::vec2 getPosition() const;
        float getMarginWidth() const;
        void move(const glm::vec2& offset);
        void setMarginWidth(const float& width);
        void setMarginColor(const Color& color);
        void setPosition(const glm::vec2& position);
        void setFillColor(const Color& color);

        void draw(const Shape& shape) const;
    protected:
    private:
        float       __marginWidth;
        glm::vec2   __position;
        Color       __color, __marginColor;
        GLuint      __colorCode, __positionCode, __marginColorCode;
    };
}
#endif
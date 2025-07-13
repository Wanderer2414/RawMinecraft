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
        
        void move(const glm::vec2& offset);
        void setPosition(const glm::vec2& position);
        void setFillColor(const Color& color);

    protected:
        void draw(const Shape& shape) const;
    private:
        glm::vec2   __position;
        Color       __color;
        GLuint      __colorCode, __positionCode;
    };
}
#endif
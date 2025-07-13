#ifndef SHAPE_MANAGER_H
#define SHAPE_MANAGER_H
#include "Ellipse.h"
#include "Global.h"
#include "Rectangle.h"
#include "RoundedRectangle.h"
namespace MyBase {
    class ShapeManager {
    public:
        static ShapeManager& getInstance();
        static void close();
        void createShape(Rectangle& rectangle, const glm::vec2& size);
        void removeShape(Rectangle& rectangle, const glm::vec2& size);
        void createShape(Ellipse& ellipse, const glm::vec2& size);
        void removeShape(Ellipse& ellipse, const glm::vec2& size);
        void createShape(RoundedRectangle& rectangle, const glm::vec2& size, const float& radius);
        void removeShape(RoundedRectangle& rectangle, const glm::vec2& size, const float& radius);

        const GLuint& createColor(const Color& color);
        void removeColor(const Color& color);
    private:
        ShapeManager();
        ~ShapeManager();

        static ShapeManager* Default;
        std::map<std::pair<float, float>, std::pair<Rectangle*, unsigned int>> __rectangles;
        std::map<std::pair<float, float>, std::pair<Ellipse*, unsigned int>> __ellipses;
        std::map<std::tuple<float, float, float>, std::pair<RoundedRectangle*, unsigned int>> __rrectangles;
        std::map<int, std::pair<GLuint, unsigned int>> __colors;
    };
}
#endif
#ifndef MAP_MENU_H
#define MAP_MENU_H
#include "Controller2D.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyCraft {
    class MapMenu: public MyBase::Controller2D, public MyBase::ShapeContainer {
    public:
        MapMenu();
        MapMenu(const MapMenu&) = delete;
        ~MapMenu();
        void setSize(const glm::vec2& size);
        MapMenu& operator=(const MapMenu&) const = delete; 
        virtual glm::vec2       getPosition() const override,
                                getSize() const override;
    protected:
        void glDraw() const override;
    private:
        MyBase::Rectangle __bound;
        glm::vec2   __size;
        bool contains(const glm::vec2& position) const override;
    };
}
#endif
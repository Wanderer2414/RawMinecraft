#ifndef MAP_MENU_H
#define MAP_MENU_H
#include "Button.h"
#include "Container2D.h"
#include "Controller2D.h"
#include "Date.h"
#include "Font.h"
#include "Label.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyCraft {
    class MapMenuItem: public MyBase::Container2D, public MyBase::ShapeContainer {
    public:
        MapMenuItem(const MyBase::Font& font, const std::string& name, const MyBase::Date& dateCreated);
        MapMenuItem(const MapMenuItem&) = delete;
        ~MapMenuItem();
        MapMenuItem& operator=(const MapMenuItem&) const = delete; 
        glm::vec2                           getPosition() const override;
        glm::vec2                           getSize() const override;
        const std::string&                  getName() const;
        const MyBase::Date&                 getDateCreated() const;
        void setPosition(const glm::vec2& position);
        void setSize(const glm::vec2& size, const float& roundness);
        friend class MapMenu;
    protected:
    private:
        float                               __roundness;
        MyBase::RoundedRectangle            __background; 
        MyBase::Label   __worldNameLabel, __dateCreatedLabel;
        std::string     __name;
        MyBase::Date    __dateCreated;
        glm::vec2       __size;
        void __choose();
        void __unchoose();
        void glDraw() const override;
        bool contains(const glm::vec2& position) const override;
    };
    class MapMenu: public MyBase::Container2D, public MyBase::ShapeContainer {
    public:
        MapMenu();
        MapMenu(const MapMenu&) = delete;
        ~MapMenu();
        const char& getChoice() const;
        void setFont(const MyBase::Font& font);
        void setSize(const glm::vec2& size);
        void add(const std::string& name, const MyBase::Date& date);
        MapMenu& operator=(const MapMenu&) const = delete; 
        virtual glm::vec2       getPosition() const override,
                                getSize() const override;
    protected:
        void glDraw() const override;
        bool __mouseClicked(GLFWwindow*) override;
    private:
        MyBase::Rectangle   __bound;
        glm::vec2           __size;
        MyBase::Font *      __font;
        bool contains(const glm::vec2& position) const override;
        char                __index;
        std::vector<MapMenuItem*> __MapMenuItems;
    };
}
#endif
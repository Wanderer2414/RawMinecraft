#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Controller2D.h"
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
        void                    setPosition(const glm::vec2& position);
    protected:
        virtual bool            __getFocus() override, __lostFocus() override;
    private:
        float                   __roundness;
        glm::vec2               __size;
        RoundedRectangle        __bound;
        using ShapeContainer::setMarginWidth;

        bool                    contains(const glm::vec2& position) const override;
        void                    glDraw() const override;
    };
}
#endif
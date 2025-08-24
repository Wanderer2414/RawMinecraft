#ifndef LABEL_H
#define LABEL_H
#include "Controller2D.h"
#include "Font.h"
#include "Text.h"
namespace MyBase {
    class Label: public Text, public Controller2D {
    public:
        Label();
        ~Label();
        virtual glm::vec2       getPosition() const override,
                                getSize() const override;
        using  Text::setPosition;
        using  Text::move;
        virtual bool contains(const glm::vec2& position) const override;
    private:
        virtual void glDrawTransparent() const override;
    };
}
#endif
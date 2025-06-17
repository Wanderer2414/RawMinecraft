#ifndef LABEL_H
#define LABEL_H
#include "Controller.h"
#include "Font.h"
namespace MyBase {
    class Label: public Controller {
    public:
        Label(const Font& font);
        ~Label();
        virtual void setText(const std::string& text);
        virtual void setColor(const glm::vec3& color);
        virtual void setPosition(const float& x, const float& y);
    private:
        std::string __text;
        glm::vec3   __color;
        glm::vec2   __position;
        const MyBase::Font& __font;
        virtual void glDraw() const override;
    };
}
#endif
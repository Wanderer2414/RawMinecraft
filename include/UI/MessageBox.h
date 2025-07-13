#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H
#include "Button.h"
#include "Clock.h"
#include "Container2D.h"
namespace MyBase {
    class MessageBox: public Container2D, private ShapeContainer {
    public:
        MessageBox();
        ~MessageBox();

        glm::vec2       getPosition() const override,
                        getSize() const override;
        void setSize(const glm::vec2& size);
        void setPosition(const glm::vec2& position);

       void open(GLFWwindow* window);
       void close();
       
    protected:
    private:
        Clock           __sensitiveClock;
        glm::vec2       __position, __size;
        bool __isOpen;
        bool contains(const glm::vec2& position) const override;
        bool catchEvent(GLFWwindow* window) override;
        void glDraw() const override;
    };
}
#endif
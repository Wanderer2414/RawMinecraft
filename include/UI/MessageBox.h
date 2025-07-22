#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H
#include "Clock.h"
#include "Container2D.h"
#include "RoundedRectangle.h"
#include "Shape.h"
namespace MyBase {
    class MessageBox: public Container2D, public ShapeContainer {
    public:
        MessageBox();
        MessageBox(const MessageBox&) = delete;
        ~MessageBox();
        MessageBox& operator=(const MessageBox&) = delete;

        glm::vec2       getPosition() const override,
                        getSize() const override;
        void setSize(const glm::vec2& size, const float& roundness);

       void open(GLFWwindow* window);
       void close();
       
    protected:
    private:
        RoundedRectangle    __background;
        Clock               __sensitiveClock;
        float               __roundness;
        glm::vec2           __size;
        bool                __isOpen;
        bool contains(const glm::vec2& position) const override;
        bool catchEvent(GLFWwindow* window) override;
        void glDraw() const override;
    };
}
#endif
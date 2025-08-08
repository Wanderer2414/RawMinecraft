#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H
#include "Clock.h"
#include "Container2D.h"
#include "RoundedRectangle.h"
#include "Global.h"
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

       int open(GLFWwindow* window);
       void close();
    protected:
        virtual bool __exitCondition();
        int getReturnValue() const;
        void setReturnValue(const int& returnValue);
        virtual void __open(GLFWwindow*), __close(GLFWwindow*);
        virtual bool catchEvent(GLFWwindow* window) override;
        virtual void glDraw() const override;
        virtual void glDrawTransparent() const override;
    private:
        bool                __isOpen;
        int                 __returnValue;
        float               __roundness;
        glm::vec2           __size;
        RoundedRectangle    __background;
        bool contains(const glm::vec2& position) const override;
    };
}
#endif
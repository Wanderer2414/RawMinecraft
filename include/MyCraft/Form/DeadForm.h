#ifndef DEAD_FORM_H
#define DEAD_FORM_H
#include "Button.h"
#include "Font.h"
#include "Label.h"
#include "MessageBox.h"
namespace MyCraft {
    class DeadForm: public MyBase::MessageBox {
    public:
        DeadForm(const MyBase::Font& font);
        DeadForm(const DeadForm&) = delete;
        ~DeadForm();
        DeadForm& operator=(const DeadForm&) const = delete; 
    protected:
        bool __mouseClicked(GLFWwindow*) override;
        void __open(GLFWwindow*) override;
        void __close(GLFWwindow*) override;
    private:
        MyBase::Label                   __notices;
        MyBase::RoundedRectangleButton  __exitButton;
    };
}
#endif
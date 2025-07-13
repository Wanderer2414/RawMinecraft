#ifndef INTROFORM_H
#define INTROFORM_H
#include "Button.h"
#include "Font.h"
#include "Form.h"
namespace MyCraft {
    class IntroForm: public MyBase::Form {
    public:
        IntroForm(GLFWwindow* window, const int& index);
        ~IntroForm();
        bool catchEvent(GLFWwindow* window) override;
    private:
        MyBase::RectangleButton button;
        MyBase::Font font;
    };
}
#endif
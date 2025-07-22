#ifndef INTROFORM_H
#define INTROFORM_H
#include "Button.h"
#include "Font.h"
#include "Form.h"
#include "Label.h"
namespace MyCraft {
    class IntroForm: public MyBase::Form {
    public:
        IntroForm(GLFWwindow* window, const int& index);
        ~IntroForm();
        bool catchEvent(GLFWwindow* window) override;
    private:
        MyBase::Label                   __programmeName;
        MyBase::RoundedRectangleButton  __startButton, __settingButton, __aboutusButton, __exitButton;
        MyBase::Font font;
    };
}
#endif
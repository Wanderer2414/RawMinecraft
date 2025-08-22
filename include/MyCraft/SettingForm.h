#pragma once
#include "Button.h"
#include "Font.h"
#include "Form.h"
#include "Label.h"
namespace MyCraft {
    class SettingForm: public MyBase::Form {
    public:
        SettingForm(GLFWwindow* window, const int& index);
        ~SettingForm();
    private:
        bool __mouseClicked(GLFWwindow*) override;
        bool catchEvent(GLFWwindow*) override;
        MyBase::Label                   __programmeName;
        MyBase::RoundedRectangleButton  __startButton, __settingButton, __aboutusButton, __exitButton;
        MyBase::Font font;
    };
}

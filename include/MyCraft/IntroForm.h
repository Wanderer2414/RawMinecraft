#ifndef INTROFORM_H
#define INTROFORM_H
#include "Button.h"
#include "Font.h"
#include "Form.h"
#include "Label.h"
#include "../UI/Slider.h"
#include "../UI/VerticalBar.h"
#include "UI/Sound.h"
#include "UI/Texture.h"
namespace MyCraft {
    class IntroForm: public MyBase::Form {
    public:
        IntroForm(GLFWwindow* window, const int& index);
        ~IntroForm();
    private:
        bool __mouseClicked(GLFWwindow*) override;
        bool catchEvent(GLFWwindow*) override;
        MyBase::Label                   __programmeName;
        MyBase::TextureViewer           __background;
        MyBase::Texture                 __bgtexture;
        MyBase::RoundedRectangleButton  __startButton, __settingButton, __aboutusButton, __exitButton;
        MyBase::Font font;
    };
}
#endif
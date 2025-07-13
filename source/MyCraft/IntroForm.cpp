#include "IntroForm.h"
#include "Form.h"

namespace MyCraft {
    IntroForm::IntroForm(GLFWwindow* window, const int& index): MyBase::Form(index), font("assets/fonts/Oswald-Regular.ttf") {
        setBackgroundColor(BLACK);
        button.setFont(font);
        button.setTextColor(WHITE);
        button.setSize({0.3, 0.2});
        button.setNormalColor({100, 100, 100, 255});
        button.setHoverColor({50, 50, 50, 255});
        button.setClickColor(RED);
        button.setText("Play");
        button.setPosition({-0.15,-0.1});
        button.setScale({1, 32});
        insert(&button);
    }
    IntroForm::~IntroForm() {

    }
    bool IntroForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Form::catchEvent(window);
        if (button.isPressed()) setReturnForm(1);
        return is_changed;
    }
}
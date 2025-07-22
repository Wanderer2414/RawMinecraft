#include "IntroForm.h"
#include "Form.h"

namespace MyCraft {
    IntroForm::IntroForm(GLFWwindow* window, const int& index): MyBase::Form(index), font("assets/fonts/SyneMono-Regular.ttf") {
        setBackgroundColor(BLACK);

        __programmeName.setFont(font);
        __programmeName.setText("My Craft");
        __programmeName.setScale({0.15,0.25});
        __programmeName.setPosition({-__programmeName.getSize().x/2, 0.5});
        __programmeName.setTextColor(WHITE);

        __startButton.setFont(font);
        __startButton.setTextColor(WHITE);
        __startButton.setNormalColor({100, 100, 100, 255});
        __startButton.setHoverColor({50, 50, 50, 255});
        __startButton.setClickColor(RED);
        __startButton.setText("Start");
        __startButton.setSize({0.6, 0.12}, 0.01);
        __startButton.setPosition({-0.3, 0});
        __startButton.setScale({0.04, 0.04});

        __settingButton.setFont(font);
        __settingButton.setTextColor(WHITE);
        __settingButton.setNormalColor({100, 100, 100, 255});
        __settingButton.setHoverColor({50, 50, 50, 255});
        __settingButton.setClickColor(RED);
        __settingButton.setText("Settings");
        __settingButton.setSize({0.6, 0.12}, 0.01);
        __settingButton.setPosition({-0.3,-0.17});
        __settingButton.setScale({0.04, 0.04});

        __aboutusButton.setFont(font);
        __aboutusButton.setTextColor(WHITE);
        __aboutusButton.setNormalColor({100, 100, 100, 255});
        __aboutusButton.setHoverColor({50, 50, 50, 255});
        __aboutusButton.setClickColor(RED);
        __aboutusButton.setText("About us");
        __aboutusButton.setSize({0.6, 0.12}, 0.01);
        __aboutusButton.setPosition({-0.3,-0.34});
        __aboutusButton.setScale({0.04, 0.04});

        __exitButton.setFont(font);
        __exitButton.setTextColor(WHITE);
        __exitButton.setNormalColor({100, 100, 100, 255});
        __exitButton.setHoverColor({50, 50, 50, 255});
        __exitButton.setClickColor(RED);
        __exitButton.setText("Exit");
        __exitButton.setSize({0.6, 0.12}, 0.01);
        __exitButton.setPosition({-0.3,-0.51});
        __exitButton.setScale({0.04, 0.04});

        insert(&__programmeName);
        insert(&__startButton);
        insert(&__settingButton);
        insert(&__aboutusButton);
        insert(&__exitButton);
    }
    IntroForm::~IntroForm() {

    }
    bool IntroForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Form::catchEvent(window);
        if (__startButton.isPressed()) setReturnForm(1);
        return is_changed;
    }
}
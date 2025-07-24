#include "IntroForm.h"
#include "Color.h"
#include "ControlCenter.h"
#include "Form.h"
#include "GLFW/glfw3.h"

namespace MyCraft {
    IntroForm::IntroForm(GLFWwindow* window, const int& index): MyBase::Form(index), font("assets/fonts/SyneMono-Regular.ttf") {
        setBackgroundColor(BLACK);

        __programmeName.setFont(font);
        __programmeName.setText("My Craft");
        __programmeName.setScale({0.2,0.3});
        __programmeName.setPosition({-__programmeName.getSize().x/2, 0.5});
        __programmeName.setTextColor(WHITE);

        __startButton.setFont(font);
        __startButton.setTextColor(WHITE);
        __startButton.setNormalColor(GRAY);
        __startButton.setHoverColor(DARKGRAY);
        __startButton.setClickColor(GRAY);
        __startButton.setText("Start");
        __startButton.setSize({0.6, 0.12}, 0.01);
        __startButton.setPosition({-0.3, 0});
        __startButton.setScale({0.06, 0.06});

        __settingButton.setFont(font);
        __settingButton.setTextColor(WHITE);
        __settingButton.setNormalColor(GRAY);
        __settingButton.setHoverColor(DARKGRAY);
        __settingButton.setClickColor(GRAY);
        __settingButton.setText("Settings");
        __settingButton.setSize({0.6, 0.12}, 0.01);
        __settingButton.setPosition({-0.3,-0.17});
        __settingButton.setScale({0.06, 0.06});

        __aboutusButton.setFont(font);
        __aboutusButton.setTextColor(WHITE);
        __aboutusButton.setNormalColor(GRAY);
        __aboutusButton.setHoverColor(DARKGRAY);
        __aboutusButton.setClickColor(GRAY);
        __aboutusButton.setText("About us");
        __aboutusButton.setSize({0.6, 0.12}, 0.01);
        __aboutusButton.setPosition({-0.3,-0.34});
        __aboutusButton.setScale({0.06, 0.06});

        __exitButton.setFont(font);
        __exitButton.setTextColor(WHITE);
        __exitButton.setNormalColor(GRAY);
        __exitButton.setHoverColor(DARKGRAY);
        __exitButton.setClickColor(GRAY);
        __exitButton.setText("Exit");
        __exitButton.setSize({0.6, 0.12}, 0.01);
        __exitButton.setPosition({-0.3,-0.51});
        __exitButton.setScale({0.06, 0.06});

        insert(&__programmeName);
        insert(&__startButton);
        insert(&__settingButton);
        insert(&__aboutusButton);
        insert(&__exitButton);
    }
    IntroForm::~IntroForm() {

    }
    bool IntroForm::__mouseClicked(GLFWwindow*) {
        if (__startButton.isPressed()) {
            setReturnValue(1);
            close();
        }
        if (__exitButton.isPressed()) {
            setReturnValue(-1);
            close();
        }
        return false;
    }
    bool IntroForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Form::catchEvent(window);
        if (MyBase::ControlCenter::getInstance().IsKeyPressed() && glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            is_changed = false;
            setReturnValue(-1);
            close();
        }
        return is_changed;
    }
}
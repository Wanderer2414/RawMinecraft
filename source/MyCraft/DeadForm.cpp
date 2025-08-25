#include "DeadForm.h"
#include "ControlCenter.h"
#include "Font.h"
namespace MyCraft {
    DeadForm::DeadForm(const MyBase::Font& font) {
        setSize({0.6, 0.4}, 0.01);
        setPosition({-0.3, -0.2});
        setFillColor(WHITE);
        setReturnValue(0);

        __notices.setFont(font);
        __notices.setText("You're dead!");
        __notices.setTextColor(RED);
        __notices.setScale({0.1, 0.1});
        __notices.setPosition({-__notices.getSize().x/2.f, 0.02});

        __exitButton.setFont(font);
        __exitButton.setText("Respawn");
        __exitButton.setScale({0.05, 0.06});
        __exitButton.setPosition({-0.25, -0.19});
        __exitButton.setSize({0.5, 0.1}, 0.01);
        __exitButton.setTextColor(BLACK);
        __exitButton.setNormalColor({200, 200, 200, 255});
        __exitButton.setHoverColor({100, 100, 100, 255});
        __exitButton.setClickColor({50, 50, 50, 255});

        insert(&__notices);
        insert(&__exitButton);
    }
    DeadForm::~DeadForm() {}
    bool DeadForm::__mouseClicked(GLFWwindow*) {
        if (__exitButton.isPressed()) {
            setReturnValue(0);
            close();
        }
        return false;
    }
    
    void DeadForm::__open(GLFWwindow* window) {
        MyBase::ControlCenter::getInstance().BindSubScreen();
        MyBase::ControlCenter::EnableMouse(window);
    }
    void DeadForm::__close(GLFWwindow* window) {
        if (!getReturnValue()) {
            MyBase::ControlCenter::DisableMouse(window);
            MyBase::ControlCenter::CenteringMouse(window);
        }
    }
}
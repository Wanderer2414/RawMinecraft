#include "CreateWorldForm.h"
#include "Color.h"
#include "ControlCenter.h"
namespace MyBase {
    CreateWorldForm::CreateWorldForm(const Font& font): __isSubmit(false) {
        setSize({0.8, 0.3}, 0.01);
        setPosition({-0.4, -0.15});
        setFillColor(WHITE);

        __nameLabel.setPosition({-0.35, 0.01});
        __nameLabel.setFont(font);
        __nameLabel.setTextColor(BLACK);
        __nameLabel.setScale({0.04,0.08});
        __nameLabel.setText("Name: ");
        insert(&__nameLabel);
 
        __nameInput.setFont(font);
        __nameInput.setTextColor(BLACK);
        __nameInput.setPosition({-0.23, -0.01});
        __nameInput.setScale({0.051,0.08});
        __nameInput.setSize({0.61, 0.1}, 0.01);
        __nameInput.setFillColor({200, 200, 200, 255});
        insert(&__nameInput);

        __dateLabel.setPosition({-0.35, -0.10});
        __dateLabel.setFont(font);
        __dateLabel.setScale({0.04,0.06});
        __dateLabel.setText("Created date: " + std::format("{}:{}:{}", __date.getDay(), __date.getMonth(), __date.getYear()));
        __dateLabel.setTextColor(BLACK);
        insert(&__dateLabel, 0);

        __createButton.setPosition({0.15, -0.13});
        __createButton.setSize({0.23, 0.1}, 0.01);
        __createButton.setNormalColor(LIGHTGRAY);
        __createButton.setHoverColor(GRAY);
        __createButton.setClickColor(LIGHTGRAY);
        __createButton.setFont(font);
        __createButton.setTextColor(BLACK);
        __createButton.setText("Create");
        __createButton.setScale({0.05,0.05});
        insert(&__createButton, 0);
    }
    CreateWorldForm::~CreateWorldForm() {}

    std::string CreateWorldForm::getName() const {
        return __nameInput.getText();
    }
    Date CreateWorldForm::getDate() const {
        return __date;
    }
    bool CreateWorldForm::isSubmit() const {
        return __isSubmit;
    }
    bool CreateWorldForm::__mouseClicked(GLFWwindow*) {
        if (__createButton.isPressed()) {
            __isSubmit = true;
            close();
            return true;
        }
        return false;
    }
    void CreateWorldForm::clear() {
        __isSubmit = false;
        __nameInput.setText("");
    }
    void CreateWorldForm::__open(GLFWwindow*) {
        ControlCenter::getInstance().BindSubScreen()
;    }
}
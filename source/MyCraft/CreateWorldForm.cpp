#include "CreateWorldForm.h"
#include "Color.h"
namespace MyBase {
    CreateWorldForm::CreateWorldForm(const Font& font) {
        setSize({0.8, 0.3}, 0.01);
        setPosition({-0.4, -0.15});
        setFillColor(WHITE);

        __nameLabel.setPosition({-0.35, 0.01});
        __nameLabel.setFont(font);
        __nameLabel.setScale({0.025,0.05});
        __nameLabel.setText("Name: ");
        insert(&__nameLabel);

        __nameInput.setPosition({-0.22, 0});
        __nameInput.setSize({0.6, 0.1}, 0.01);
        __nameInput.setFillColor({200, 200, 200, 255});
        insert(&__nameInput);

        __dateLabel.setPosition({-0.35, -0.10});
        __dateLabel.setFont(font);
        __dateLabel.setScale({0.025,0.05});
        __dateLabel.setText("Created date: " + std::format("{}:{}:{}", __date.getDay(), __date.getMonth(), __date.getYear()));
        __dateLabel.setTextColor(BLACK);
        insert(&__dateLabel, 0);

        __createButton.setPosition({0.15, -0.13});
        __createButton.setSize({0.23, 0.1}, 0.01);
        __createButton.setNormalColor({200, 200, 200, 255});
        __createButton.setHoverColor({150, 150, 150, 255});
        __createButton.setClickColor({200, 200, 200, 100});
        __createButton.setFont(font);
        __createButton.setTextColor(BLACK);
        __createButton.setText("Create");
        __createButton.setScale({0.03,0.04});
        insert(&__createButton, 0);
    }
    CreateWorldForm::~CreateWorldForm() {}
}
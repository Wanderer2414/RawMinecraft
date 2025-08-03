#include "MediateForm.h"
#include "Color.h"
#include "ControlCenter.h"
#include "Date.h"
#include "Form.h"
#include "GameForm.h"
#include "General.h"

namespace MyCraft {
    MediateForm::MediateForm(GLFWwindow* window, const int& index): 
        Form(index), __font("assets/fonts/SyneMono-Regular.ttf"),
        __createWorldForm(__font), __waitingCreateMap(__font)
        {
        setBackgroundColor(BLACK);
        __createWorldButton.setFont(__font);
        __createWorldButton.setTextColor(WHITE);
        __createWorldButton.setNormalColor(GRAY);
        __createWorldButton.setHoverColor(DARKGRAY);
        __createWorldButton.setClickColor(GRAY);
        __createWorldButton.setText("Create");
        __createWorldButton.setSize({0.3, 0.12}, 0.01);
        __createWorldButton.setPosition({0.6, -0.48});
        __createWorldButton.setScale({0.06, 0.06});
        insert(&__createWorldButton);

        __clearWorldButton.setFont(__font);
        __clearWorldButton.setTextColor(WHITE);
        __clearWorldButton.setNormalColor(GRAY);
        __clearWorldButton.setHoverColor(DARKGRAY);
        __clearWorldButton.setClickColor(GRAY);
        __clearWorldButton.setText("Clear");
        __clearWorldButton.setSize({0.3, 0.12}, 0.01);
        __clearWorldButton.setPosition({0.6, -0.62});
        __clearWorldButton.setScale({0.06, 0.06});
        insert(&__clearWorldButton);

        __connectWorldButton.setFont(__font);
        __connectWorldButton.setTextColor(WHITE);
        __connectWorldButton.setNormalColor(GRAY);
        __connectWorldButton.setHoverColor(DARKGRAY);
        __connectWorldButton.setClickColor(GRAY);
        __connectWorldButton.setText("Connect");
        __connectWorldButton.setSize({0.3, 0.12}, 0.01);
        __connectWorldButton.setPosition({0.6, -0.76});
        __connectWorldButton.setScale({0.06, 0.06});
        insert(&__connectWorldButton);

        __joinWorldButton.setFont(__font);
        __joinWorldButton.setTextColor(WHITE);
        __joinWorldButton.setNormalColor(GRAY);
        __joinWorldButton.setHoverColor(DARKGRAY);
        __joinWorldButton.setClickColor(GRAY);
        __joinWorldButton.setText("Join");
        __joinWorldButton.setSize({0.3, 0.12}, 0.01);
        __joinWorldButton.setPosition({0.6, -0.9});
        __joinWorldButton.setScale({0.06, 0.06});
        insert(&__joinWorldButton);

        __menu.setFont(__font);
        __menu.setPosition({-0.9, -0.9});
        __menu.setSize({1.48, 0.54});
        __menu.setFillColor(TRANSPARENCY);
        __menu.setMarginWidth(2);
        __menu.setMarginColor(WHITE);
        insert(&__menu);

        __returnButton.setSize({0.2, 0.1}, 0.01);
        __returnButton.setPosition({-0.9, 0.85});
        __returnButton.setNormalColor(GRAY);
        __returnButton.setHoverColor(DARKGRAY);
        __returnButton.setClickColor(GRAY);
        __returnButton.setTexture("assets/images/arrow_back.png");
        __returnButton.setTextureExportSize({0.08,0.08});
        __returnButton.setTextureOrigin({0, 0});
        __returnButton.setTextureImportSize({1.0/3,1});
        insert(&__returnButton);

        __mapTexture.setTextureExportPosition({-0.9,-0.3});
        __mapTexture.setTextureExportSize({2.0/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1});
        __mapTexture.setTextureImportSize({1, 1});
        insert(&__mapTexture);
        auto& list = __worldsManage.getWorld();
        for (const auto& info: list) {
            __menu.add(info->getWorldName(), info->getCreatedDate());
        }
    }

    MediateForm::~MediateForm() {}

    bool MediateForm::__mouseClicked(GLFWwindow* window) {
        bool is_changed = false;
        if (__createWorldButton.isPressed()) {
            pauseScreen(window);
            __createWorldForm.open(window);
            if (__createWorldForm.isSubmit()) {
                std::string world_name = __createWorldForm.getName();
                MyBase::Date date = __createWorldForm.getDate();
                if (world_name.empty()) world_name = "Newworld";
                __menu.add(world_name, date);
                int index = __worldsManage.createWorld(world_name, date);
                std::string file = "bin/"+std::to_string(index)+"/";
                MyBase::CreateFolder(file);
                __waitingCreateMap.open(file, window);
                __worldsManage.save();
            }
        }
        if (__menu.isPressed()) {
            int index = __menu.getChoice();
            if (index != -1) {
                std::string file = "bin/"+std::to_string(index)+"/overal.png";
                __mapTexture.setTexture(file);
                is_changed = true;
            }
        }
        if (__joinWorldButton.isPressed()) {
            int index = __menu.getChoice();
            if (index != -1) {
                std::string file = "bin/"+std::to_string(index)+"/";

                GameForm gameForm(window, 0, file);
                gameForm.run(window);
                
                setReturnValue(1);
                close();
            }
        }
        if (__returnButton.isPressed()) {
            setReturnValue(0);
            close();
        }
        return is_changed;
    }
    bool MediateForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Form::catchEvent(window);
        if (MyBase::ControlCenter::getInstance().IsKeyPressed() && glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            setReturnValue(0);
            close();
            is_changed = false;
        }
        return is_changed;
    }
}
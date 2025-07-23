#include "MediateForm.h"
#include "ChunkLoader.h"
#include "Color.h"
#include "Date.h"
#include "Form.h"
#include "GameForm.h"
#include "General.h"

namespace MyCraft {
    MediateForm::MediateForm(GLFWwindow* window, const int& index): Form(index), __font("assets/fonts/SyneMono-Regular.ttf"), __createWorldForm(__font) {
        setBackgroundColor(BLACK);

        __createWorldButton.setFont(__font);
        __createWorldButton.setTextColor(WHITE);
        __createWorldButton.setNormalColor({100, 100, 100, 255});
        __createWorldButton.setHoverColor({50, 50, 50, 255});
        __createWorldButton.setClickColor(RED);
        __createWorldButton.setText("Create");
        __createWorldButton.setSize({0.3, 0.12}, 0.01);
        __createWorldButton.setPosition({0.6, -0.48});
        __createWorldButton.setScale({0.06, 0.06});
        insert(&__createWorldButton);

        __clearWorldButton.setFont(__font);
        __clearWorldButton.setTextColor(WHITE);
        __clearWorldButton.setNormalColor({100, 100, 100, 255});
        __clearWorldButton.setHoverColor({50, 50, 50, 255});
        __clearWorldButton.setClickColor(RED);
        __clearWorldButton.setText("Clear");
        __clearWorldButton.setSize({0.3, 0.12}, 0.01);
        __clearWorldButton.setPosition({0.6, -0.62});
        __clearWorldButton.setScale({0.06, 0.06});
        insert(&__clearWorldButton);

        __connectWorldButton.setFont(__font);
        __connectWorldButton.setTextColor(WHITE);
        __connectWorldButton.setNormalColor({100, 100, 100, 255});
        __connectWorldButton.setHoverColor({50, 50, 50, 255});
        __connectWorldButton.setClickColor(RED);
        __connectWorldButton.setText("Connect");
        __connectWorldButton.setSize({0.3, 0.12}, 0.01);
        __connectWorldButton.setPosition({0.6, -0.76});
        __connectWorldButton.setScale({0.06, 0.06});
        insert(&__connectWorldButton);

        __joinWorldButton.setFont(__font);
        __joinWorldButton.setTextColor(WHITE);
        __joinWorldButton.setNormalColor({100, 100, 100, 255});
        __joinWorldButton.setHoverColor({50, 50, 50, 255});
        __joinWorldButton.setClickColor(RED);
        __joinWorldButton.setText("Join");
        __joinWorldButton.setSize({0.3, 0.12}, 0.01);
        __joinWorldButton.setPosition({0.6, -0.9});
        __joinWorldButton.setScale({0.06, 0.06});
        insert(&__joinWorldButton);

        __menu.setFont(__font);
        __menu.setPosition({-0.9, -0.9});
        __menu.setSize({1.48, 0.54});
        __menu.setFillColor(TRANSPARENT);
        __menu.setMarginWidth(2);
        __menu.setMarginColor(WHITE);
        insert(&__menu);

        auto& list = __worldsManage.getWorld();
        for (const auto& info: list) {
            __menu.add(info->getWorldName(), info->getCreatedDate());
        }
    }

    MediateForm::~MediateForm() {}

    bool MediateForm::__mouseRelease(GLFWwindow* window) {
        if (__createWorldButton.isReleased()) {
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
                ChunkLoader::create(file);
            }
        }
        if (__joinWorldButton.isReleased()) {
            int index = __menu.getChoice();
            if (index >= 0) {
                std::string file = "bin/"+std::to_string(index)+"/";
                GameForm gameForm(window, 0, file);
                int form_index = gameForm.run(window);
                setReturnForm(form_index);
            }
        }
        return false;
    }
}
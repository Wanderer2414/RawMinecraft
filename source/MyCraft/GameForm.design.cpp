#include "GameForm.h"
#include "Biome.h"
#include "File.h"
#include "Form3D.h"
#include "GamePauseForm.h"
#include "HitBoxCenter.h"
#include "ControlCenter.h"

namespace MyCraft {
    GameForm::GameForm(GLFWwindow* window, const int& index, const std::string& src): 
        Form3D(index), pWorld(0, 0, 0, src), __pauseForm(__font), 
        __font("assets/fonts/SyneMono-Regular.ttf"), __biomeManage(src) 
    {
        setBackgroundColor(BLACK);
        insert(&pWorld);
        insert(&__model);
        insert(&__hitbox);
        insert(&__label);
        insert(&__biomeLabel);
        __hitbox.insert(&__model);
        __hitbox.match(&__model);
        __hitbox.match(&pWorld);
        __hitbox.match(&camera);

        __label.setFont(__font);
        __label.setText("Max fps:");
        __label.setTextColor(RED);
        __label.setScale({0.03, 0.06});
        __label.setPosition({0.8f, 0.95f});

        __biomeLabel.setFont(__font);
        __biomeLabel.setText("Max fps:");
        __biomeLabel.setTextColor(RED);
        __biomeLabel.setScale({0.03, 0.06});
        __biomeLabel.setPosition({0.8f, 0.9f});

        MyBase::ControlCenter::DisableMouse(window);
        MyBase::ControlCenter::CenteringMouse(window);
    
        camera.setPosition({10, 10, 1.7});
        pZVelocity = 0;
        pSpeed = 0.1;
        pFrameAlarm.setDuration(50);
        std::cout << "Open time: " << 1.0f*clock()/CLOCKS_PER_SEC << std::endl;

        __positionLabel.setFont(__font);
        __positionLabel.setPosition({-0.9, 0.9});
        __positionLabel.setTextColor(RED);
        __positionLabel.setScale({0.04, 0.06});
        insert(&__positionLabel);

        __fpsClock.setDuration(500);

        MyBase::File file(src+"info.bin");
        if (!file.isNew()) {
            glm::ivec2 pos;
            file >> pos.x >> pos.y;
            __model.teleport(glm::vec3(pos, 100));
            pWorld.playerAt(glm::vec3(pos, 100));
        }
        else {
            glm::ivec2 pos(0,0);
            file << pos.x << pos.y;
            __model.teleport(glm::vec3(pos, 100));
            pWorld.playerAt(glm::vec3(pos, 100));
        }
        file.close();
    }
    GameForm::~GameForm() {        
    }
    bool GameForm::move(const float& x, const float& y, const float& z) {
        glm::vec3 delta = {0, 0, 0}, pos= camera.getCameraPosition();
        delta += x*camera.getHorizontalVector();
        glm::vec3 tmp = camera.getDirection();
        tmp.z = 0;
        tmp /= glm::length(tmp);
        delta.x += y*tmp.x;
        delta.y += y*tmp.y;
        delta.z += z;
        return true;
    }
    bool GameForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase3D::Form3D::catchEvent(window);
        if (MyBase::ControlCenter::getInstance().IsKeyPressed()) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
                pauseScreen(window);
                MyBase::ControlCenter::EnableMouse(window);
                int value = __pauseForm.open(window);
                if (!value) {
                    MyBase::ControlCenter::DisableMouse(window);
                    MyBase::ControlCenter::CenteringMouse(window);
                    is_changed = true;
                    update();
                }
                else if (value == 1) close();
            }
        }
        return is_changed;
    }
    bool GameForm::handle(GLFWwindow* window) {
        bool is_changed = Form3D::handle(window);
        if (__fpsClock.get()) {
            __fpsClock.restart();
            __label.setText(std::format("Fps: {}", (int)getCurrentFps()));
            __biomeLabel.setText(to_string(__biomeManage.getBiome(__model.getModelPosition()).type));
            is_changed = true;
        }
    
        glm::vec2 delta = MyBase::ControlCenter::getInstance().getCursorPos(window);
    
        if (delta.x != 0 || delta.y != 0) {
            MyBase::ControlCenter::CenteringMouse(window);
            __model.seeRotate(-delta.x, delta.y);
            is_changed = true;
        }
        if (__model.isRun()) {
            __positionLabel.setText(std::format("Position: ({}, {}, {})", (int)__model.getModelPosition().x, (int)__model.getModelPosition().y, (int)__model.getModelPosition().z));
            is_changed = true;
        }
        return is_changed;
    }
}
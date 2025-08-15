#include "GameForm.h"
#include "Bag.h"
#include "Biome.h"
#include "Camera.h"
#include "Container2D.h"
#include "Container3D.h"
#include "File.h"
#include "Form3D.h"
#include "GamePauseForm.h"
#include "ControlCenter.h"
#include "InventoryElement.h"
#include "Message.h"

namespace MyCraft {
    GameForm::GameForm(GLFWwindow* window, const int& index, const std::string& src):
        Form3D(index), __world(0, 0, 0, src), __pauseForm(__font),
        __font("assets/fonts/SyneMono-Regular.ttf"), __biomeManage(src), 
        __inventoryForm(__inventory, __model.getItems()) 
    {
        setBackgroundColor(BLACK);
        insert(&__world);
        insert(&__model);
        insert(&__label);
        insert(&__biomeLabel);
        insert(&__model);
        insert(&__cursor);
        insertPermanent(&__inventory);
        MyBase::Network::match(&__model);
        MyBase::Network::match(&__world);
        MyBase::Network::match(&__inventoryForm);
        MyBase::Network::match(&__inventory);
        MyBase::Network::match(&__sun);
        MyBase::Network::match(&__model);
        MyBase::Network::match(&__healthBar);
        MyBase::Network::match(&MyBase3D::Camera::Instance());

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
    
        MyBase3D::Camera::Instance().setPosition({10, 10, 1.7});
        __zVelocity = 0;
        __speed = 0.1;
        __frameAlarm.setDuration(50);
        std::cout << "Open time: " << 1.0f*clock()/CLOCKS_PER_SEC << std::endl;

        __positionLabel.setFont(__font);
        __positionLabel.setPosition({-0.9, 0.9});
        __positionLabel.setTextColor(RED);
        __positionLabel.setScale({0.04, 0.06});
        insert(&__positionLabel);
        insert(&__healthBar);
        __fpsClock.setDuration(500);
        MyBase::File file(src+"info.bin");
        if (!file.isNew()) {
            glm::ivec3 pos;
            file >> pos.x >> pos.y >> pos.z;
            __spawnPoint = pos;
        }
        else {
            glm::ivec3 pos(0,0,0);
            file << pos.x << pos.y << pos.z;
        }
        
        file.close();
    }
    GameForm::~GameForm() {   
        MyBase::Network::close();     
    }
    void GameForm::__open(GLFWwindow* window) {
        __model.teleport(__spawnPoint);
        __world.teleport(__spawnPoint);
    }
    bool GameForm::move(const float& x, const float& y, const float& z) {
        glm::vec3 delta = {0, 0, 0}, pos= MyBase3D::Camera::Instance().getCameraPosition();
        delta += x*MyBase3D::Camera::Instance().getHorizontalVector();
        glm::vec3 tmp = MyBase3D::Camera::Instance().getDirection();
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
                int value = __pauseForm.open(window);
                if (!value) {
                    is_changed = true;
                    update();
                }
                else if (value == 1) close();
            }
            else if (glfwGetKey(window, GLFW_KEY_E)) {
                InventoryUI* ui = new Bag(__model.getItems());
                __inventoryForm.setDefaultUI(ui);
                __inventoryForm.open(window);
                ui->close();
                __inventoryForm.setDefaultUI(0);
                delete ui;
                
                is_changed = true;
            }
        }
        return is_changed;
    }
    bool GameForm::handle(GLFWwindow* window) {
        bool is_changed = Form3D::handle(window);
        is_changed = __sun.handle(window) || is_changed;
        if (__fpsClock.get()) {
            __fpsClock.restart();
            __label.setText(std::format("Fps: {}", (int)getCurrentFps()));
            glm::vec3 position = __model.getModelPosition();
            position.x = floor(position.x/16);
            position.y = floor(position.y/16);
            position.z = floor(position.z/16);
            __biomeLabel.setText(to_string(__biomeManage.getBiome(position).type));
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
    void GameForm::update() {
        MyBase::Container2D::update();
        MyBase3D::Container3D::update();
    }
    void GameForm::glDraw() const {
        __sun.glDraw();
        MyBase3D::Form3D::glDraw();
    }
}
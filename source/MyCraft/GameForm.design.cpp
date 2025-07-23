#include "GameForm.h"
#include "Form3D.h"
#include "GamePauseForm.h"
#include "HitBoxCenter.h"
#include "ControlCenter.h"

namespace MyCraft {
    GameForm::GameForm(GLFWwindow* window, const int& index, const std::string& src): Form3D(index), pWorld(0, 0, 0, src) {
        setBackgroundColor(BLACK);
        insert(&pWorld);
        insert(&__model);
        insert(&__hitbox);
        insert(&__label);
        __hitbox.insert(&__model);
        __hitbox.match(&__model);
        __hitbox.match(&pWorld);
        __hitbox.match(&camera);
        __font.loadFont("assets/fonts/SyneMono-Regular.ttf");
        __label.setFont(__font);
        __label.setText("Max fps:");
        __label.setTextColor(RED);
        __label.setScale({0.03, 0.06});
        __label.setPosition({0.7f, 0.95f});

        MyBase::ControlCenter::getInstance().DisableMouse(window);
    
        camera.setPosition({10, 10, 1.7});
        pZVelocity = 0;
        pSpeed = 0.1;
        pFrameAlarm.setDuration(50);
        std::cout << "Open time: " << 1.0f*clock()/CLOCKS_PER_SEC << std::endl;
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
                MyBase::ControlCenter::getInstance().EnableMouse(window);
                GamePauseForm form;
                form.open(window);
                MyBase::ControlCenter::getInstance().DisableMouse(window);
                is_changed = true;
                update();
            }
        }
        return is_changed;
    }
    bool GameForm::handle(GLFWwindow* window) {
        bool is_changed = Form3D::handle(window);
        __label.setText("Max fps: " + std::to_string(getMaxFps()));
        glm::vec3 dir = __model.getDirection();
        dir.z -=2;
    
        glm::vec<2, double> position;
        glfwGetCursorPos(window, &position.x, &position.y);
        glm::vec2 delta = position;
        delta -= MyBase::ControlCenter::getInstance().getWindowHalf();
    
        if (delta.x != 0 || delta.y != 0) {
            glfwSetCursorPos(window, MyBase::ControlCenter::getInstance().getWindowHalf().x, MyBase::ControlCenter::getInstance().getWindowHalf().y);
            __model.seeRotate(-delta.x/1000, -delta.y/1000);
            is_changed = true;
        }
        return is_changed;
    }
}
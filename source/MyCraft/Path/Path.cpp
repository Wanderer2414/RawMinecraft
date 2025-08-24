#include "Path.h"
#include "ModelController.h"
namespace MyCraft {
    Path::Path(): __speed(0.05), __controller(0), __cur(0), __end(0), __size(0) {
        __clockSpeed.setDuration(30);
    }
    Path::~Path() {
        while (__cur) {
            Node* tmp = __cur;
            __cur = __cur->next;
            delete tmp;
        }
    }

    void Path::push(const glm::vec3& postition) {
        if (!__cur) {
            __cur = __end = new Node();
            __cur->position = postition;
        }
        else {
            __end->next = new Node();
            __end->next->position = postition;
            __end = __end->next;
        }
        __size++;
    }
    int Path::size() const {
        return __size;
    }
    const glm::vec3& Path::getEnd() const {
        if (!__end) throw std::runtime_error("Null pointer!");
        else return __end->position;
    }
    const glm::vec3& Path::getCur() const {
        if (!__cur) throw std::runtime_error("Null pointer");
        else return __cur->position;
    }
    void Path::setModelHost(ModelController* controller) {
        __controller = controller;
    }
    void Path::setSpeed(const float& speed) {
        __speed = speed;
    }
    Path::Node::Node(): next(0) {}
    bool Path::handle(GLFWwindow* window) {
        bool is_changed = false;
        if (__clockSpeed.get()) {
            __clockSpeed.restart();
            glm::vec3 delta(__cur->next->position);
            delta  -= __controller->getPosition();
            delta.z = 0;
            while (__controller && __cur && __cur->next && glm::length(delta)<0.4) {
                Node* tmp = __cur;
                __cur = __cur->next;
                __size--;
                delete tmp;
                if (__size==1) {
                    delete __cur;
                    __cur = __end = 0;
                    __size = 0;
                }
            }
            if (__controller && __cur && __cur->next) {
                glm::vec3 __position = __controller->getPosition();
                delta = glm::normalize(delta)*__speed;
                __controller->move(delta);
                if (glm::length(__position-__controller->getPosition())<0.005) {
                    while (__cur) {
                        Node* tmp = __cur;
                        __cur = __cur->next;
                        delete tmp;
                    }
                    __cur = __end = 0;
                    __size = 0;
                }
                is_changed = true;
            }
        }
        return is_changed;
    }
}
#ifndef PATH_H
#define PATH_H
#include "Clock.h"
#include "Controller3D.h"
#include "ModelController.h"
namespace MyCraft {
    class Path: public MyBase3D::Controller3D {
    public:
        Path();
        ~Path();

        void setModelHost(ModelController* controller);
        void setSpeed(const float& speed);
        friend class PathCreator;
    protected:
    private:
        struct Node {
            glm::vec3 position;
            Node* next;
            Node();
        };
        Node            *__cur, *__end;
        float           __speed;
        ModelController* __controller;
        MyBase::Clock   __clockSpeed;
        bool handle(GLFWwindow* window) override;
    };
}
#endif
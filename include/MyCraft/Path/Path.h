#ifndef PATH_H
#define PATH_H
#include "Clock.h"
#include "ModelController.h"
namespace MyCraft {
    class Path {
    public:
        Path();
        ~Path();

        void push(const glm::vec3& postition);
        int size() const;
        const glm::vec3& getEnd() const;
        const glm::vec3& getCur() const;

        void setModelHost(ModelController* controller);
        void setSpeed(const float& speed);
        bool handle(GLFWwindow* window);
    protected:
    private:
        struct Node {
            glm::vec3 position;
            Node* next;
            Node();
        };
        Node            *__cur, *__end;
        int             __size;
        float           __speed;
        ModelController* __controller;
        MyBase::Clock   __clockSpeed;
    };
}
#endif
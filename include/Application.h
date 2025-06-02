#ifndef APPLICATION_H
#define APPLICATION_H
#include "Global.h"
namespace MyCraft {
    class Application {
        public:
            Application(const float& width, const float& height);
            ~Application();
            void run();
        private:
            GLFWwindow* __window;
    };
};
#endif
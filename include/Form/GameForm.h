#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Global.h"
namespace MyCraft {
    class GameForm {
        public:
            GameForm();
            ~GameForm();
            void run(GLFWwindow* window);
        private:
    };
}
#endif
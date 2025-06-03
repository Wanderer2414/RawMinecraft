#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Camera.h"
#include "Clock.h"
#include "Form3D.h"
#include "Global.h"
#include "World.h"

namespace MyCraft {

    class GameForm: public MyBase3D::Form3D {
        public:
            GameForm(GLFWwindow* window, const int& index);
            ~GameForm();
        
            bool move(const float& x, const float& y, const float& z);
        protected:
        private:
            float           pZVelocity;
            bool            handle(GLFWwindow* window) override;
            MyBase::Clock   pFrameAlarm;
            World           pWorld;
            float           pSpeed;
        };
}
#endif
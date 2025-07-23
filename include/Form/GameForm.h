#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Camera.h"
#include "Clock.h"
#include "Font.h"
#include "Form3D.h"
#include "HitBoxCenter.h"
#include "Label.h"
#include "ModelController.h"
#include "PlayerModelController.h"
#include "World.h"

namespace MyCraft {

    class GameForm: public MyBase3D::Form3D {
        public:
            GameForm(GLFWwindow* window, const int& index, const std::string& src);
            ~GameForm();
        
            bool move(const float& x, const float& y, const float& z);
        protected:
        private:
            float           pZVelocity;
            bool            catchEvent(GLFWwindow* window) override;
            bool            handle(GLFWwindow* window) override;
            MyBase::Font    __fontDefault;
            MyBase::Clock   pFrameAlarm;
            MyBase::Font    __font;
            MyBase::Label   __label;
            World           pWorld;
            PlayerModelController     __model;
            HitBoxCenter    __hitbox;
            float           pSpeed;
        };
}
#endif
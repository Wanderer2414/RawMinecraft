#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Biome.h"
#include "Camera.h"
#include "Clock.h"
#include "Font.h"
#include "Form3D.h"
#include "GamePauseForm.h"
#include "HitBoxCenter.h"
#include "Inventory.h"
#include "InventoryForm.h"
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
            MyBase::Clock   __frameAlarm, __fpsClock;
            MyBase::Font    __font;
            MyBase::Label   __label, __positionLabel, __biomeLabel;
            World           __world;
            PlayerModelController     __model;
            BiomeManage     __biomeManage;
            HitBoxCenter    __hitbox;
            float           __speed;
            GamePauseForm   __pauseForm;
            Inventory       __inventory;
            InventoryForm   __inventoryForm;
        };
}
#endif
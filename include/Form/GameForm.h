#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Biome.h"
#include "Camera.h"
#include "Clock.h"
#include "Cursor.h"
#include "Font.h"
#include "Form3D.h"
#include "GamePauseForm.h"
#include "HitBoxCenter.h"
#include "InteractiveForm.h"
#include "Inventory.h"
#include "InventoryForm.h"
#include "Item.h"
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
            void update() override;
        protected:
        private:
            float           pZVelocity;
            bool            catchEvent(GLFWwindow* window) override;
            bool            handle(GLFWwindow* window) override;
            Cursor          __cursor;
            MyBase::Font    __fontDefault;
            MyBase::Clock   __frameAlarm, __fpsClock;
            MyBase::Font    __font;
            MyBase::Label   __label, __positionLabel, __biomeLabel;
            World           __world;
            PlayerModelController __model;
            BiomeManage     __biomeManage;
            float           __speed;
            GamePauseForm   __pauseForm;
            Inventory       __inventory;
            InventoryForm   __inventoryForm;
        };
}
#endif
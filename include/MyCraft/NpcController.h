#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H

#include "ModelController.h"
#include <glm/glm.hpp>
#include <random>

namespace MyCraft {
    class NpcController : public ModelController {
        public:
            NpcController();
            ~NpcController();
            void see(const glm::vec3& dir) override;
            void move(const glm::vec3& dir) override;
            void rotate(const float& angle) override;
            glm::vec3 getModelPosition() const override;
            glm::mat4x3 getShape() const override;
        protected:
            void update() override;
            virtual void performBehavior() = 0; // Hành vi đặc trưng của mỗi NPC
            glm::vec3 position; // Vị trí hiện tại
            glm::mat4x3 hitbox; // Hình dạng/hitbox
            float moveSpeed; // Tốc độ di chuyển
            std::mt19937 rng;
    };
};

#endif
#ifndef MODEL_CONTROLLER_MODEL_H
#define MODEL_CONTROLLER_MODEL_H
#include "MyCraft/Model/ModelController.h"
#include "SkeletonBoss/Model.h"

namespace MyCraft {
    namespace SkeletonBoss {
        class ModelController: public MyCraft::ModelController,public SkeletonBoss::Model {
        public:
            ModelController();
            ModelController(const ModelController&) = delete;
            ~ModelController();
            ModelController& operator=(const ModelController&) const = delete; 

            virtual void    see(const glm::vec3& dir) override,
                            look(const glm::vec3& position) override,
                            move(const glm::vec3& dir) override,
                            rotate(const glm::vec3& angle) override;
            void            update() override;
            glm::vec3       getPosition() const override;
            glm::mat4x3     getShape() const override;
            void            damage(const unsigned int& damage) override;
            void            heal(const unsigned int& health) override;
        protected:
        private:
            virtual void    __see(const glm::vec3& dir) override,
                            __look(const glm::vec3& position) override,
                            __move(const glm::vec3& dir) override,
                            __rotate(const glm::vec3& angle) override;

            void            __dead() override;
            void            __damage() override;
            void            __heal() override;

            virtual void    glDrawTransparent() const override;

            bool __isChanged, __isDamage;
            float __speed;
            MyBase::Clock __damageDuration, __fallCheckClock;
            bool handle(GLFWwindow* window) override;
            void glDraw() const override;
        };
    }
}
#endif
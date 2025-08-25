#ifndef PIG_CONTROLLER_H
#define PIG_CONTROLLER_H
#include "Clock.h"
#include "Model/ModelController.h"
#include "Pig/Model.h"
namespace MyCraft {
    namespace Pig {
        class Controller: public ModelController, private Pig::Model {
        public:
            Controller();
            ~Controller();
            void    setPosition(const glm::vec3& position) override;
            void    see(const glm::vec3& dir) override,
                    move(const glm::vec3& dir) override,
                    look(const glm::vec3& position) override,
                    rotate(const glm::vec3& angle) override;
            void    damage(const unsigned int& damage) override;
            void    heal(const unsigned int& health) override;

            void    update() override;
            glm::vec3 getPosition() const override;
            glm::mat4x3 getShape() const override;
            void save(std::ostream& cout) override;
        private:
            bool    __isChanged, __isDamage;
            float   __speed;
            MyBase::Clock __damageDuration, __fallCheckClock;
            bool handle(GLFWwindow* window) override;
            void glDraw() const override;

            void    __see(const glm::vec3& dir) override,
                    __move(const glm::vec3& dir) override,
                    __look(const glm::vec3& pos) override,
                    __rotate(const glm::vec3& angle) override;
            void    __dead()                        override;
            void    __damage()                      override;
            void    __heal()                        override;
            void __load(std::istream& cin)      override;
        };
    }
}
#endif
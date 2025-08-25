#ifndef ZOMBIE_CONTROLLER_H
#define ZOMBIE_CONTROLLER_H
#include "ChunkManage.h"
#include "Clock.h"
#include "Message.h"
#include "Model/ModelController.h"
#include "Zombie/Model.h"
namespace MyCraft {
    namespace Zombie {
        class Controller: public ModelController, private Model {
        public:
            Controller();
            ~Controller();

            float   Powerness() const override;
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
            MyBase::Clock __damageDuration, __fallCheckClock, __freeTime, __updateFollowClock;
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

        class FocusCommand: public  MyBase::Command {
        public:
            FocusCommand(MyCraft::ModelController* model);
            ~FocusCommand();

            MyBase::MessageType getType()                               const override;
            void execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   override;
        private:
            MyCraft::ModelController*      __model;
        };
    }
}
#endif
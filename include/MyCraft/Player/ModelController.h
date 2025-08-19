#ifndef PLAYER_MODEL_CONTROLLER_H
#define PLAYER_MODEL_CONTROLLER_H
#include "Block.h"
#include "Clock.h"
#include "HealthModule.h"
#include "Item.h"
#include "Message.h"
#include "InventoryModule.h"
#include "Model/ModelController.h"
#include "Player/Model.h"

namespace MyCraft {
    namespace Player {
        class ModelController: public MyCraft::ModelController, public InventoryModule, public HealthModule, public Player::Model {
        public: 
            ModelController();
            ~ModelController();
            glm::vec3   getPosition() const    override,
                        getDirection() const;
            void        see(const glm::vec3& dir) override,
                        move(const glm::vec3& dir) override,
                        look(const glm::vec3& dir) override,
                        rotate(const glm::vec3& angle) override;
                                
            void        teleport(const glm::vec3& position),
                        rightAttack(),
                        leftAttack(),
                        seeRotate(const float& horizontal, const float& vertical),
                        setDrawAble(const bool& drawable),
                        glDraw() const override,
                        
                        setHoverBlock(const glm::vec3& hover, const glm::vec3& place, const BlockCatogary& type);
            glm::mat4x3 getShape() const override;
            friend class MoveCommand;
        private:
            bool            __isDrawable, __isChanged;
            float           __speed;
            glm::vec3       __direction, __eye_direction;
            MyBase::Clock   __speedControl;
            BlockCatogary   __type;
            glm::vec3       __toAbsoluteCoordinate(const glm::vec3& dir) const;
            bool            __moveManage(GLFWwindow* window);
            void            __damage() override;
            void            __dead() override;
            void            __heal() override;
            void            __move(const glm::vec3& delta) override,
                            __rotate(const glm::vec3& dir)  override,
                            __look(const glm::vec3& position) override,
                            __see(const glm::vec3& dir) override;
            
            bool            handle(GLFWwindow* window) override;
            bool            catchEvent(GLFWwindow* window) override;
            void            reset() override;
            void            update() override;
        };

        class ResetCameraCommand: public MyBase::Command {
        public:
            ResetCameraCommand(Player::ModelController* model);
            ~ResetCameraCommand();
            MyBase::MessageType getType() const override;
            void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
        private:
            Player::ModelController* __model;
        };


        class MoveCommand: public MyBase::Command {
        public:
            MoveCommand(Player::ModelController* model);
            ~MoveCommand();

            MyBase::MessageType getType()                               const override;
            void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
        private:
            Player::ModelController*      __model;
        };
        class DiveCommand: public MyBase::Command {
        public:
            DiveCommand(Player::ModelController& model);
            ~DiveCommand();
            MyBase::MessageType getType()                               const override;
            void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
        private:
            Player::ModelController& __model;
        };

        class OnGroundCommand: public MyBase::Command {
        public:
            OnGroundCommand(Player::ModelController& model);
            ~OnGroundCommand();
            MyBase::MessageType getType()                               const override;
            void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message)   override;
        private:
            Player::ModelController& __model;
        };
    }
}
#endif
#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller.h"
#include "Message.h"

namespace MyCraft {
    class HitBoxCenter;
    class ModelController: public MyBase::Controller {
        public:
            ModelController();
            ~ModelController();
            bool isFall() const;
            float getZVelocity() const;
            virtual void    see(const glm::vec3& dir) = 0,
                            move(const glm::vec3& dir) = 0,
                            rotate(const float& angle) = 0;
            virtual glm::vec3 getModelPosition() const = 0;
            virtual glm::mat4x3 getShape() const = 0;
            friend class MoveCommand;
            friend class FallCommand;
            friend class StopFallCommand;
        protected:
            virtual void update() = 0;
            void setZVelocity(const float& z);
        private:
            bool    __isFall;
            float   __zVelocity;
            void setFall(const bool& isFall);
    };
    class MoveCommand: public Command {
    public:
        MoveCommand(ModelController* model);
        ~MoveCommand();

        MessageType getType()                               const override;
        void execute(Port& mine, Port& source, Message* message)   override;
    private:
        ModelController*      __model;
    };
    class FallCommand: public Command {
    public:
        FallCommand(ModelController* model);
        ~FallCommand();

        MessageType getType()                               const override;
        void execute(Port& mine, Port& source, Message* message)   override;
    private:
        ModelController*      __model;
    };
    class StopFallCommand: public Command {
    public:
        StopFallCommand(ModelController* model);
        ~StopFallCommand();

        MessageType getType()                               const override;
        void execute(Port& mine, Port& source, Message* message)   override;
    private:
        ModelController*      __model;
    };
    
}
#endif
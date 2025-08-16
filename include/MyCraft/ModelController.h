#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"
#include "Message.h"

namespace MyCraft {
    class HitBoxCenter;
    class ModelController: public MyBase3D::Controller3D {
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
            friend class JumpCommand;
        protected:
            virtual void update() = 0;
            void setZVelocity(const float& z);
        private:
            bool    __isFall;
            float   __zVelocity;
            void setFall(const bool& isFall);
    };
    class MoveCommand: public  MyBase::Command {
    public:
        MoveCommand(MyCraft::ModelController* model);
        ~MoveCommand();

         MyBase::MessageType getType()                               const override;
        void execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   override;
    private:
        MyCraft::ModelController*      __model;
    };
    class FallCommand: public  MyBase::Command {
    public:
        FallCommand(MyCraft::ModelController* model);
        ~FallCommand();

         MyBase::MessageType getType()                               const override;
        void execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   override;
    private:
        MyCraft::ModelController*      __model;
    };
    class StopFallCommand: public  MyBase::Command {
    public:
        StopFallCommand(MyCraft::ModelController* model);
        ~StopFallCommand();

        MyBase::MessageType getType()                               const override;
        void execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   override;
    private:
        MyCraft::ModelController*      __model;
    };
    class JumpCommand: public MyBase::Command {
    public:
        JumpCommand(ModelController* model);
        ~JumpCommand();

        MyBase::MessageType getType()                               const override;
        void execute( MyBase::Port& mine,  MyBase::Port& source,  MyBase::Message* message)   override;
    private:
        MyCraft::ModelController*      __model;
    };
    
}
#endif
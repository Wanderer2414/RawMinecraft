#ifndef MODEL_H
#define MODEL_H
#include "Clock.h"
#include "Controller3D.h"
#include "Message.h"

namespace MyCraft {
    class HitBoxCenter;
    class ModelController: public MyBase3D::Controller3D, public MyBase::Port {
        public:
            ModelController();
            ~ModelController();
            float getZVelocity() const;
            virtual glm::vec3 getPosition() const = 0;
            virtual glm::mat4x3 getShape() const = 0;
            
            virtual void    see(const glm::vec3& dir) = 0,
                            move(const glm::vec3& dir) = 0,
                            rotate(const glm::vec3& angle) = 0;
                            
            friend class MoveCommand;
            friend class FallCommand;
            friend class StopFallCommand;
            friend class JumpCommand;
        protected:
            virtual void update() = 0;
            void setZVelocity(const float& z);
        private:
            float   __zVelocity;
            virtual void    __see(const glm::vec3& dir) = 0,
                            __move(const glm::vec3& dir) = 0,
                            __rotate(const glm::vec3& angle) = 0;
    };

    class MoveMessage: public MyBase::Message {
    public:
        MoveMessage(const glm::vec3& direction, const float& Depth);
        ~MoveMessage();
        MyBase::MessageType   getType() const override;
        const glm::vec3       direction;
        const float           depth;
    };
    class FallMessage: public MyBase::Message {
    public:
        FallMessage(const float& zVelocity);
        ~FallMessage();
        MyBase::MessageType     getType() const override;

        const float zVelocity;
    };
    class StopFallMessage: public MyBase::Message {
    public:
        StopFallMessage();
        ~StopFallMessage();
        MyBase::MessageType     getType() const override;
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
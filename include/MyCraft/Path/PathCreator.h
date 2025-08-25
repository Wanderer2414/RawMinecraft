#ifndef PATH_CREATOR_H
#define PATH_CREATOR_H
#include "Message.h"
#include "ModelController.h"
#include "Path/Path.h"
#include "WorldRender.h"
namespace MyCraft {
    class PathCreator: public MyBase::Port {
    public:
        PathCreator(WorldRender& world);
        ~PathCreator();

        Path* FollowPath(const glm::vec3& size, const glm::vec3& source, const glm::vec3& destination)  const;
        Path* PatrolPath(const glm::vec3& size, const glm::vec3& source)                                const;
        Path* RunAwayPath(const glm::vec3& size, const glm::vec3& source)                               const;
        void Pair(Path& path)                                                   const;
    protected:
    private:
        WorldRender& __worldRender;
        bool containable(const glm::vec3& size, const glm::vec3& source) const;
        bool canGotoStraight(const glm::vec3& source, const glm::vec3& size, const glm::vec3& direction) const;
    };

    class CreatePatrolPathMessage: public MyBase::Message {
    public:
        CreatePatrolPathMessage(ModelController* controller);
        ~CreatePatrolPathMessage();

        MyBase::MessageType getType() const override;
        ModelController* controller;
    };
    class CreatePatrolPathCommand: public MyBase::Command {
    public:
        CreatePatrolPathCommand(PathCreator& creator);
        ~CreatePatrolPathCommand();

        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
        
    private:
        PathCreator& __creator;
    };

    class CreateFollowPathMessage: public MyBase::Message {
    public:
        CreateFollowPathMessage(ModelController* controller, const glm::vec3& destination);
        ~CreateFollowPathMessage();

        MyBase::MessageType getType() const override;
        ModelController* controller;
        const glm::vec3 destination;
    };
    class CreateFollowPathCommand: public MyBase::Command {
    public:
        CreateFollowPathCommand(PathCreator& creator);
        ~CreateFollowPathCommand();

        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
        
    private:
        PathCreator& __creator;
    };
    
}
#endif
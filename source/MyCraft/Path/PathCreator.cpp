#include "PathCreator.h"
#include "Block.h"
#include "Message.h"
#include "Path.h"

namespace MyCraft {
    PathCreator::PathCreator(WorldRender& world): __worldRender(world) {
        add(new CreatePatrolPathCommand(*this));
        add(new CreateFollowPathCommand(*this));
    }
    PathCreator::~PathCreator() {}

    Path* PathCreator::FollowPath(const glm::vec3& size, const glm::vec3& source, const glm::vec3& destination)  const {
        Path* path = new Path();
        path->push(source);
        path->push(destination);
        return path;
    }
    Path* PathCreator::PatrolPath(const glm::vec3& size, const glm::vec3& source) const {
        int numberOfNode = rand()%5+1;
        float angle = (rand()%100)/50.f*M_PI;
        glm::vec2 direction(3*cos(angle), 3*sin(angle));
        angle -= M_PI_4*3;
        Path* path = new Path();
        path->push(source);
        for (int i = 0; i<numberOfNode; i++) {
            int count = 0;
            while (count < 8 && !canGotoStraight(path->getEnd(), size, path->getEnd() + glm::vec3(direction, 0))) {
                angle = angle + M_PI_4;
                direction = {cos(angle), sin(angle)};
                count++;
            }
            if (count == 6) break;
            else {
                path->push(path->getEnd()+glm::vec3(direction, 0));
                angle -= M_PI_4*3;
            }
        }
        if (path->size()<=1) {
            delete path;
            path = 0;
        }
        return path;
    }
    Path* PathCreator::RunAwayPath(const glm::vec3& size, const glm::vec3& source) const {
        return 0;
    }
    void PathCreator::Pair(Path& path) const {
    }
    bool PathCreator::containable(const glm::vec3& size, const glm::vec3& source) const {
        bool canContain = true;
        glm::ivec2 xBound(floor(source.x-size.x/2), ceil(source.x+size.y/2));
        glm::ivec2 yBound(floor(source.y-size.y/2), ceil(source.y+size.y/2));
        glm::ivec2 zBound(floor(source.z), ceil(source.z+size.z));
        for (glm::vec3 position(xBound.x, yBound.x, zBound.x); position.x<=xBound.y && canContain; position.x++) {
            for (position.y = yBound.x; position.y<=yBound.y && canContain; position.y++) {
                for (position.z = zBound.x; position.z <= zBound.y && canContain; position.z++) {
                    if (__worldRender.contains(position))
                        canContain = canContain && isPlaceable(__worldRender.getType(position));
                    else canContain = false;
                }
            }
        }
        return canContain;
    }
    bool PathCreator::canGotoStraight(const glm::vec3& source, const glm::vec3& size, const glm::vec3& direction) const {
        glm::vec3 delta = direction/10.f, cur = source;
        bool canGoto = true;
        for (int i = 0; i<=10 && canGoto; i++) {
            cur = cur+delta;
            cur.z = __worldRender.getZHeight(cur);
            if (cur.z<16*7) canGoto = canGoto && containable(size, cur);
        }
        return canGoto;
    }

    CreatePatrolPathMessage::CreatePatrolPathMessage(ModelController* c): controller(c) {}
    CreatePatrolPathMessage::~CreatePatrolPathMessage() {}

    MyBase::MessageType CreatePatrolPathMessage::getType() const {
        return MyBase::CreatePatrolPath;
    }
    
    CreatePatrolPathCommand::CreatePatrolPathCommand(PathCreator& creator): __creator(creator) {}
    CreatePatrolPathCommand::~CreatePatrolPathCommand() {}

    MyBase::MessageType CreatePatrolPathCommand::getType() const {
        return MyBase::CreatePatrolPath;
    }
    void CreatePatrolPathCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        CreatePatrolPathMessage* package= (CreatePatrolPathMessage*)(message);

        glm::mat4x3 mat = package->controller->getShape();
        glm::vec3 size = {glm::length(mat[1]), glm::length(mat[2]), glm::length(mat[3])};

        package->controller->setPath(__creator.PatrolPath(size, package->controller->getPosition()));
    }

    CreateFollowPathMessage::CreateFollowPathMessage(ModelController* c, const glm::vec3& d): controller(c), destination(d) {}
    CreateFollowPathMessage::~CreateFollowPathMessage() {}

    MyBase::MessageType CreateFollowPathMessage::getType() const {
        return MyBase::CreateFollowPath;
    }
    
    CreateFollowPathCommand::CreateFollowPathCommand(PathCreator& creator): __creator(creator) {}
    CreateFollowPathCommand::~CreateFollowPathCommand() {}

    MyBase::MessageType CreateFollowPathCommand::getType() const {
        return MyBase::CreateFollowPath;
    }
    void CreateFollowPathCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        CreateFollowPathMessage* package= (CreateFollowPathMessage*)(message);

        glm::mat4x3 mat = package->controller->getShape();
        glm::vec3 size = {glm::length(mat[1]), glm::length(mat[2]), glm::length(mat[3])};
        Path* path = __creator.FollowPath(size, package->controller->getPosition(), package->destination);
        if (package->controller->hasPath()) package->controller->clearPath();
        package->controller->setPath(path);
        path->setSpeed(0.13);
    }
}
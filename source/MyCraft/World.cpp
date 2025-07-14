#include "World.h"
#include "Block.h"
#include "Container3D.h"
#include "DrawingCenter.h"
#include "Message.h"
#include "General.h"
#include "PlayerModelController.h"

namespace MyCraft {
    World::World(const int& x, const int& y, const int& z): __isHoverBlock(false) {
        // __worldRender.createPlaneWorld();
        // exit(0);
        __worldRender.load({-16*3,-16*3,-16*2});
        insert(&__worldRender);
        pFrameAlarm.setDuration(150);
        add(new CheckFallCommand(this));
        add(new CheckEmptyCommand(this));
        add(new CheckHoverCommand(this));
        add(new PlaceblockCommand(this));
        add(new WorldMoveCommand(this));
    }
    World::~World() {
    }
    
    bool World::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window);
        if (pFrameAlarm.get()) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
            }
        }
        return is_changed;
    }
    const BlockCatogary::Catogary& World::at(const glm::vec3& pos) const {
        return __worldRender.at(pos);
    }

    void World::set(const int& rx, const int& ry, const int& rz, const BlockCatogary::Catogary& type) {
        __worldRender.place({rx,ry,rz}, type);
    }

    void World::set(const glm::vec3& pos, const BlockCatogary::Catogary& type) {
        set(std::floor(pos.x), std::floor(pos.y), std::floor(pos.z), type);
    }
    void World::setHoverBlock(const glm::vec3& pos, const glm::vec3& placePosition) {
        __hoverBlock = pos;
        __placePosition = placePosition;
        __isHoverBlock = true;
    }
    void World::unHoverBlock() {
        __isHoverBlock = false;
    }
    void World::playerAt(const glm::vec3& pos) {
        glm::vec3 position = pos - glm::vec3(16,16,16)*2.f;
        __worldRender.load(position);
    }
    void World::glDraw() const {
        MyBase3D::Container3D::glDraw();
        if (__isHoverBlock) {
            DrawingCenter::getInstance().BindMargin();
            glm::vec4 vec = glm::vec4(__hoverBlock,1);
            DrawingCenter::getInstance().DrawMargin((void*)&vec, 1, 2);
        }
    }
    CheckEmptyCommand::CheckEmptyCommand(World* world): __world(world) {}
    CheckEmptyCommand::~CheckEmptyCommand() {Command::~Command();}
    
    void CheckEmptyCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestGotoMessage* request = (RequestGotoMessage*)message;
        bool below_result = true, above_result = true;
        auto shape = request->rectangleBox;
        glm::vec3 dir = glm::vec3(request->direction, 0);
        //Below check
        glm::vec3 npos = shape[0] + dir, epos = npos + shape[1];
        std::queue<glm::vec3> q = rasterize(npos, epos);
        while (q.size() && below_result) {
            if (__world->at(q.front()) != BlockCatogary::Air) below_result = false;
            q.pop();
        }
        //Above block check
        npos.z += 1;
        epos.z += 1;
        q = rasterize(npos, epos);
        while (q.size() && above_result) {
            if (__world->at(q.front()) != BlockCatogary::Air) above_result = false;
            q.pop();
        }
        if (!below_result || !above_result) {
            //Check auto jump
            if (above_result) {
                below_result = above_result = true;
                q = rasterize(shape[0]-glm::vec3(0,0,1), shape[0]+shape[1]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world->at(q.front()) == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                q = rasterize(shape[0]+shape[2]-glm::vec3(0,0,1), shape[0]+shape[1]+shape[2]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world->at(q.front()) == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z += 1;
                epos.z += 1;
                q = rasterize(npos, epos);
                while (q.size() && above_result) {
                    if (__world->at(q.front()) != BlockCatogary::Air) above_result = false;
                    q.pop();
                }
            }
            if (below_result && above_result) dir.z=1;
            else {
                //Check parallel Ox
                npos = shape[0] + glm::vec3(dir.x, 0, 0); epos = npos + shape[1];
                below_result = above_result = true;
                q = rasterize(npos, epos);
                while (q.size() && below_result) {
                    if (__world->at(q.front()) != BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z++; epos.z++;
                q = rasterize(npos, epos);
                while (q.size() && below_result) {
                    if (__world->at(q.front()) != BlockCatogary::Air) above_result = false;
                    q.pop();
                }

                if (above_result && below_result) dir.y = 0;
                else {
                    //Check paralel Oy
                    npos = shape[0] + glm::vec3(0, dir.y, 0); epos = npos + shape[1];
                    below_result = above_result = true;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world->at(q.front()) != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    npos.z++; epos.z++;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world->at(q.front()) != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    if (below_result) dir.x = 0;
                }
            }
        }
        if (below_result) {
            shape[3] = shape[0] + shape[2];
            shape[3][2]--;
            shape[2] += shape[0] + shape[1];
            shape[2][2]--;
            shape[1] += shape[0];
            shape[1][2]--;
            shape[0][2]--;

            if (__world->at(shape[0])==BlockCatogary::Air && 
                __world->at(shape[1])==BlockCatogary::Air && 
                __world->at(shape[2])==BlockCatogary::Air && 
                __world->at(shape[3])==BlockCatogary::Air) {
                    dir.z = -0.01;
            }

            mine.send(source, new MoveMessage(dir));
        }
    }
    MyBase::MessageType CheckEmptyCommand::getType() const {
        return MyBase::MessageType::RequestGoto;
    }
    
    CheckFallCommand::CheckFallCommand(World* world): __world(world) {}
    CheckFallCommand::~CheckFallCommand() {}
        
    void CheckFallCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestFallMessage* request = (RequestFallMessage*)message;
        float z = request->zVelocity;
        auto shape = request->rectangleBox;
        if (z<=0) {
            z -= 0.06;
            bool isFall = true;
            shape[3] = shape[0] + shape[2];
            isFall = isFall && (__world->at(shape[3]+glm::vec3(0,0,z))==BlockCatogary::Air || 
                                __world->at(shape[3])!=BlockCatogary::Air); 
            shape[2] += shape[0] + shape[1];
            isFall = isFall && (__world->at(shape[2]+glm::vec3(0,0,z))==BlockCatogary::Air || 
                                __world->at(shape[2])!=BlockCatogary::Air); 
            shape[1] += shape[0];
            isFall = isFall && (__world->at(shape[1]+glm::vec3(0,0,z))==BlockCatogary::Air || 
                                __world->at(shape[1])!=BlockCatogary::Air); 

            isFall = isFall && (__world->at(shape[0]+glm::vec3(0,0,z))==BlockCatogary::Air || 
                                __world->at(shape[0])!=BlockCatogary::Air); 
            if (isFall) mine.send(source, new FallMessage(z));
            else {
                float delta = shape[0][2] - floor(shape[0][2]);
                mine.send(source, new FallMessage(-delta));
                mine.send(source, new StopFallMessage());
            }
        }
        else if (z>0) {
            shape[0] += shape[3];

            shape[3] = shape[0] + shape[2];
            shape[3][2]+=0.1;
            shape[2] += shape[0] + shape[1];
            shape[2][2]+=0.1;
            shape[1] += shape[0];
            shape[1][2]+=0.1;
            shape[0][2]+=0.1;
            if (__world->at(shape[0])==BlockCatogary::Air && 
                __world->at(shape[1])==BlockCatogary::Air && 
                __world->at(shape[2])==BlockCatogary::Air && 
                __world->at(shape[3])==BlockCatogary::Air) {
                    mine.send(source, new FallMessage(z-0.035));
            }
            else {
                float delta = floor(shape[0][2]) - shape[0][2];
                if (delta>=0.01) delta -= 0.01;
                mine.send(source, new FallMessage(delta));
            }
        }
    }
    MyBase::MessageType CheckFallCommand::getType() const {
        return MyBase::MessageType::RequestFall;
    }

    CheckHoverCommand::CheckHoverCommand(World* world): __world(world) {}
    CheckHoverCommand::~CheckHoverCommand() {}
    MyBase::MessageType CheckHoverCommand::getType() const {
        return MyBase::MessageType::CheckHover;
    };
    void CheckHoverCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        CheckHoverMessage* package = (CheckHoverMessage*)message;
        __world->__cameraPosition = package->position + glm::vec3(0,0,1.8);
        __world->__cameraDir = package->direction;
        auto q = rasterize(__world->__cameraPosition, __world->__cameraPosition+package->direction*4.f);
        bool hover = false;
        glm::vec3 placePosition;
        while (q.size() && !hover) {
            if (__world->at(q.front())!=BlockCatogary::Air) {
                hover = true;
            }
            else {
                placePosition = q.front();
                q.pop();
            }
        }
        if (hover) {
            __world->setHoverBlock(q.front(), placePosition);
        }
        else __world->unHoverBlock();
    }
    PlaceblockCommand::PlaceblockCommand(World* world): __world(world) {}
    PlaceblockCommand::~PlaceblockCommand() {}
    MyBase::MessageType PlaceblockCommand::getType() const {
        return MyBase::MessageType::RightAttack;
    }
    void PlaceblockCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        RightAttackMessage* package = (RightAttackMessage*)message;
        glm::vec3 fpos(floor(package->posistion.x), floor(package->posistion.y), floor(package->posistion.z));
        if (__world->__isHoverBlock && __world->__placePosition!=fpos) {
            fpos.z += 1;
            if (__world->__placePosition != fpos)
                __world->set(__world->__placePosition, BlockCatogary::Grass);
        }
    }


    WorldMoveMessage::WorldMoveMessage(const glm::vec3& pos): position(pos) {}
    WorldMoveMessage::~WorldMoveMessage() {}
    MyBase::MessageType WorldMoveMessage::getType() const {
        return MyBase::WorldMove;
    }

    WorldMoveCommand::WorldMoveCommand(MyCraft::World* world): __world(world) {}
    WorldMoveCommand::~WorldMoveCommand() {}
    MyBase::MessageType WorldMoveCommand::getType() const {
        return MyBase::WorldMove;
    }
    void WorldMoveCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        WorldMoveMessage& package = *(WorldMoveMessage*)message;
        __world->playerAt(package.position);
    };
}
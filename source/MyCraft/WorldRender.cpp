#include "WorldRender.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkBase.h"
#include "ChunkManage.h"
#include "DrawingCenter.h"
#include "General.h"
#include "Message.h"
#include "PlayerModelController.h"

namespace MyCraft {
    WorldRender::WorldRender(const std::string& src): __chunkLoader(src), __isHover(false) {
        insert(&__chunkLoader);
        add(new CheckEmptyCommand(*this));
        add(new CheckFallCommand(*this));
        add(new CheckHoverCommand(*this));
        add(new WorldMoveCommand(*this));
    }
    
    WorldRender::~WorldRender() {}
    bool WorldRender::contains(const glm::ivec3& pos) const {
        return __chunkLoader.contains(pos);
    }
    bool WorldRender::isHover() const {
        return __isHover;
    }
    glm::ivec3 WorldRender::getHoverBlock() const {
        return __hoverBlock;
    }
    glm::ivec3 WorldRender::getPlaceBlock() const {
        return __placePosition;
    }

    void WorldRender::place(const BlockCatogary& type) {
        if (type && isValid(type, __hoverPlane)) {
            __chunkLoader.setType(__placePosition, type);
            if (isMultiState(type)) __chunkLoader.setState(__placePosition, getState(type, __hoverPlane));
        }
    }
    void WorldRender::unplace() {
        __chunkLoader.setType(__hoverBlock, Air);
    }
    
    void WorldRender::setHoverBlock(const glm::vec3& pos, const glm::vec3& place) {
        __hoverBlock = pos;
        __placePosition = place;
        __isHover = true;
        if (place.x < pos.x) __hoverPlane = 4;
        else if (place.x > pos.x) __hoverPlane = 1;
        else if (place.z < pos.z) __hoverPlane = 5;
        else if (place.z > pos.z) __hoverPlane = 0;
        else if (place.y < pos.y) __hoverPlane = 3;
        else if (place.y > pos.y) __hoverPlane = 2;
        else __hoverPlane = 255;
    }
    void WorldRender::unHover() {
        __isHover = false;
    }
    void WorldRender::save() {}
    void WorldRender::playerAt(const glm::vec3& position) {
        __chunkLoader.playerAt(position);
    }

    const BlockCatogary& WorldRender::getType(const glm::vec3& position) const {
        glm::ivec3 pos(floor(position.x), floor(position.y), floor(position.z));
        return __chunkLoader.getType(pos);
    }
    const BlockCatogary& WorldRender::getType(const glm::vec3& position){
        glm::ivec3 pos(floor(position.x), floor(position.y), floor(position.z));
        return __chunkLoader.getType(pos);
    }
    void WorldRender::glDraw() const {
        DrawingCenter::BindMargin();
        DrawingCenter::DrawMargins((void*)__chunkLoader.getChunks().data(), __chunkLoader.getChunks().size(), RED, 2);
        Container3D::glDraw();
    }

    void WorldRender::glDrawTransparent() const {
        MyBase3D::Container3D::glDrawTransparent();
        if (__isHover) {
            glm::vec4 margin(__hoverBlock, 1);
            DrawingCenter::BindMargin();
            DrawingCenter::DrawMargins(&margin, 1, BLACK, 3);
        }
    }


    RequestGotoMessage::RequestGotoMessage(const glm::mat4x3& p, const glm::vec2& d): rectangleBox(p), direction(d) {}
    RequestGotoMessage::~RequestGotoMessage() {}

    RequestFallMessage::RequestFallMessage(const glm::mat4x3& rec, const float& z): rectangleBox(rec), zVelocity(z) {}
    RequestFallMessage::~RequestFallMessage() {
    }
    MyBase::MessageType RequestFallMessage::getType() const {
        return MyBase::MessageType::RequestFall;
    }
    MyBase::MessageType RequestGotoMessage::getType() const {
        return MyBase::MessageType::RequestGoto;
    }

    CheckEmptyCommand::CheckEmptyCommand(WorldRender& world): __world(world) {}
    CheckEmptyCommand::~CheckEmptyCommand() {}
    
    void CheckEmptyCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestGotoMessage* request = (RequestGotoMessage*)message;
        bool below_result = true, above_result = true;
        auto shape = request->rectangleBox;
        glm::vec3 dir = glm::vec3(request->direction, 0);
        //Below check
        glm::vec3 npos = shape[0] + dir, epos = npos + shape[1];
        std::queue<glm::ivec3> q = rasterize(npos, epos);
        while (q.size() && below_result) {
            if (__world.getType(q.front()) != BlockCatogary::Air) below_result = false;
            q.pop();
        }
        //Above block check
        npos.z += 1;
        epos.z += 1;
        q = rasterize(npos, epos);
        while (q.size() && above_result) {
            if (__world.getType(q.front()) != BlockCatogary::Air) above_result = false;
            q.pop();
        }
        if (!below_result || !above_result) {
            //Check auto jump
            if (above_result) {
                below_result = above_result = true;
                q = rasterize(shape[0]-glm::vec3(0,0,1), shape[0]+shape[1]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world.getType(q.front()) == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                q = rasterize(shape[0]+shape[2]-glm::vec3(0,0,1), shape[0]+shape[1]+shape[2]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world.getType(q.front()) == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z += 1;
                epos.z += 1;
                q = rasterize(npos, epos);
                while (q.size() && above_result) {
                    if (__world.getType(q.front()) != BlockCatogary::Air) above_result = false;
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
                    if (__world.getType(q.front()) != BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z++; epos.z++;
                q = rasterize(npos, epos);
                while (q.size() && below_result) {
                    if (__world.getType(q.front()) != BlockCatogary::Air) above_result = false;
                    q.pop();
                }

                if (above_result && below_result) dir.y = 0;
                else {
                    //Check paralel Oy
                    npos = shape[0] + glm::vec3(0, dir.y, 0); epos = npos + shape[1];
                    below_result = above_result = true;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world.getType(q.front()) != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    npos.z++; epos.z++;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world.getType(q.front()) != BlockCatogary::Air) below_result = false;
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

            if (__world.getType(shape[0])==BlockCatogary::Air && 
                __world.getType(shape[1])==BlockCatogary::Air && 
                __world.getType(shape[2])==BlockCatogary::Air && 
                __world.getType(shape[3])==BlockCatogary::Air) {
                    dir.z = -0.01;
            }

            mine.send(source, new MoveMessage(dir));
        }
    }
    MyBase::MessageType CheckEmptyCommand::getType() const {
        return MyBase::MessageType::RequestGoto;
    }
    
    CheckFallCommand::CheckFallCommand(WorldRender& world): __world(world) {}
    CheckFallCommand::~CheckFallCommand() {}
        
    void CheckFallCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        RequestFallMessage* request = (RequestFallMessage*)message;
        float z = request->zVelocity;
        auto shape = request->rectangleBox;
        if (z<=0) {
            z -= 0.06;
            shape[0].z += z;
            bool isFall = true;
            shape[3] = shape[0] + shape[2];
            isFall = isFall && __world.getType(shape[3])==BlockCatogary::Air; 
            shape[2] += shape[0] + shape[1];
            isFall = isFall && __world.getType(shape[2])==BlockCatogary::Air; 
            shape[1] += shape[0];   
            isFall = isFall && __world.getType(shape[1])==BlockCatogary::Air; 

            isFall = isFall && __world.getType(shape[0])==BlockCatogary::Air; 
            if (isFall) mine.send(source, new FallMessage(std::max(z, -0.8f)));
            else {
                float delta = shape[0][2] - z - ceil(shape[0][2]);
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
            if (__world.getType(shape[0])==BlockCatogary::Air && 
                __world.getType(shape[1])==BlockCatogary::Air && 
                __world.getType(shape[2])==BlockCatogary::Air && 
                __world.getType(shape[3])==BlockCatogary::Air) {
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

    CheckHoverMessage::CheckHoverMessage(const glm::vec3& pos, const glm::vec3& dir): position(pos), direction(dir) {}
    CheckHoverMessage::~CheckHoverMessage() {}
    MyBase::MessageType CheckHoverMessage::getType() const {
        return MyBase::MessageType::CheckHover;
    }

    CheckHoverCommand::CheckHoverCommand(WorldRender& world): __world(world) {}
    CheckHoverCommand::~CheckHoverCommand() {}
    MyBase::MessageType CheckHoverCommand::getType() const {
        return MyBase::MessageType::CheckHover;
    };
    void CheckHoverCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        CheckHoverMessage* package = (CheckHoverMessage*)message;
        glm::vec3 position = package->position + glm::vec3(0,0,1.8);
        glm::vec3 direction = package->direction;
        auto q = rasterize(position, position + direction*4.f, 0.05);
        bool hover = false;
        glm::ivec3 placePosition;
        while (q.size() && !hover) {
            if (__world.getType(q.front())!=BlockCatogary::Air) {
                hover = true;
            }
            else {
                placePosition = q.front();
                q.pop();
            }
        }
        if (hover) {
            __world.setHoverBlock(q.front(), placePosition);
        }
        else __world.unHover();
    }

    WorldMoveMessage::WorldMoveMessage(const glm::vec3& pos): position(pos) {}
    WorldMoveMessage::~WorldMoveMessage() {}
    MyBase::MessageType WorldMoveMessage::getType() const {
        return MyBase::WorldMove;
    }

    WorldMoveCommand::WorldMoveCommand(MyCraft::WorldRender& world): __world(world) {}
    WorldMoveCommand::~WorldMoveCommand() {}
    MyBase::MessageType WorldMoveCommand::getType() const {
        return MyBase::WorldMove;
    }
    void WorldMoveCommand::execute(MyBase::Port& mine, MyBase::Port& des, MyBase::Message* message) {
        WorldMoveMessage& package = *(WorldMoveMessage*)message;
        __world.playerAt(package.position);
    };
}
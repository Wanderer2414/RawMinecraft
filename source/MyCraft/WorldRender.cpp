#include "WorldRender.h"
#include "Block.h"
#include "ChunkManage.h"
#include "DrawingCenter.h"
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
    bool WorldRender::isBusy(const glm::vec3& position) const {
        glm::ivec3 pos(floor(position.x), floor(position.y), floor(position.z));
        BlockCatogary type = __chunkLoader.getType(pos);
        if (!isCollistion(type)) return false;
        if (!isSpecial(type)) return type;
        else {
            glm::mat4 inverse = glm::inverse(__chunkLoader.getState(pos));
            glm::vec3 cPosition = inverse*glm::vec4(position,1);
            return (cPosition.x>=0 && cPosition.x<=1 && cPosition.y>=0 && cPosition.y<=1 && cPosition.z>=0 && cPosition.z<1);
        }
    }
    bool WorldRender::isHover(const glm::vec3& position) const {
        glm::ivec3 pos(floor(position.x), floor(position.y), floor(position.z));
        BlockCatogary type = __chunkLoader.getType(pos);
        if (!isSpecial(type)) return type;
        else {
            glm::mat4 inverse = glm::inverse(__chunkLoader.getState(pos));
            glm::vec3 cPosition = inverse*glm::vec4(position,1);
            return (cPosition.x>=0 && cPosition.x<=1 && cPosition.y>=0 && cPosition.y<=1 && cPosition.z>=0 && cPosition.z<1);
        }
    }

    BlockCatogary WorldRender::getType(const glm::vec3& position) const {
        glm::ivec3 pos(floor(position.x), floor(position.y), floor(position.z));
        return __chunkLoader.getType(pos);
    }
    glm::ivec3 WorldRender::getHoverBlock() const {
        return __hoverBlock;
    }
    glm::ivec3 WorldRender::getPlaceBlock() const {
        return __placePosition;
    }

    void WorldRender::place(const BlockCatogary& type) {
        if (type == Water) __chunkLoader.pushDynamicWater(__placePosition);
        else if (type && isValid(type, __hoverPlane)) {
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
    BlockCatogary WorldRender::getHoverType() const {
        return __chunkLoader.getType(__hoverBlock);
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
        glm::vec3 delta = (epos-npos)/10.f;
        float minHeight = 0;
            
        for (int i = 0; i<=10 && below_result; i++) {
            if (__world.isBusy(npos+i*1.0f*delta)) {
                below_result = false;
                minHeight = std::max(minHeight, getSpecialState(__world.getType(npos+i*1.0f*delta))[2].z);
            }
        }
        //Above block check
        npos.z += 1;
        epos.z += 1;
        delta = (epos-npos)/10.f;
        for (int i = 0; i<=10 && above_result; i++) {
            if (__world.isBusy(npos+i*1.0f*delta)) above_result = false;
        }
        if (!below_result || !above_result) {
            //Check auto jump
            if (above_result) {
                below_result = above_result = true;
                glm::vec3 start = shape[0]-glm::vec3(0,0,1), end = shape[0]+shape[1]-glm::vec3(0,0,1);
                glm::vec3 delta = end-start;
                delta = (end-start)/10.f;
                for (int i = 0; i<10 && below_result; i++) {
                    if (!__world.isBusy(start+i*1.0f*delta)) below_result = false;
                }
                start = shape[0]+shape[2]-glm::vec3(0,0,1), end = shape[0]+shape[1]+shape[2]-glm::vec3(0,0,1);
                delta = (end-start)/10.f;
                for (int i = 0; i<=10 && below_result; i++) {
                    if (!__world.isBusy(start+i*1.0f*delta)) below_result = false;
                }
                npos.z += 1;
                epos.z += 1;
                delta = (epos-npos)/10.f;
                for (int i = 0; i<=10 && above_result; i++) {
                    if (__world.isBusy(npos+i*1.0f*delta)) above_result = false;
                }
            }
            if (below_result && above_result) 
                dir.z = minHeight;
            else {
                //Check parallel Ox
                npos = shape[0] + glm::vec3(dir.x, 0, 0); epos = npos + shape[1];
                below_result = above_result = true;
                delta = (epos-npos)/10.f;
                for (int i = 0; i<=10 && below_result; i++) {
                    if (__world.isBusy(npos+i*1.0f*delta)) below_result = false;
                }
                npos.z++; epos.z++;
                delta = (epos-npos)/10.f;
                for (int i = 0; i<=10 && above_result; i++) {
                    if (__world.isBusy(npos+i*1.0f*delta)) above_result = false;
                }

                if (above_result && below_result) dir.y = 0;
                else {
                    //Check paralel Oy
                    npos = shape[0] + glm::vec3(0, dir.y, 0); epos = npos + shape[1];
                    below_result = above_result = true;
                    delta = (epos-npos)/10.f;
                    for (int i = 0; i<=10 && below_result; i++) {
                        if (__world.isBusy(npos+i*1.0f*delta)) below_result = false;
                    }
                    npos.z++; epos.z++;
                    delta = (epos-npos)/10.f;
                    for (int i = 0; i<=10 && above_result; i++) {
                        if (__world.isBusy(npos+i*1.0f*delta)) above_result = false;
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

            if (!__world.isBusy(shape[0]) && 
                !__world.isBusy(shape[1]) && 
                !__world.isBusy(shape[2]) && 
                !__world.isBusy(shape[3])) {
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
            float minHeight = 0;
            shape[3] = shape[0] + shape[2];
            if (__world.isBusy(shape[3])) {
                isFall = false; 
                minHeight = std::max(minHeight, getSpecialState(__world.getType(shape[3]))[2].z);
            }
            shape[2] += shape[0] + shape[1];
            if (__world.isBusy(shape[2])) {
                isFall = false;
                minHeight = std::max(minHeight, getSpecialState(__world.getType(shape[2]))[2].z);
            }
            shape[1] += shape[0];   
            if (__world.isBusy(shape[1])) {
                isFall = false; 
                minHeight = std::max(minHeight, getSpecialState(__world.getType(shape[1]))[2].z);
            }

            if (__world.isBusy(shape[0])) {
                isFall = false; 
                minHeight = std::max(minHeight, getSpecialState(__world.getType(shape[0]))[2].z);
            }
            if (isFall) mine.send(source, new FallMessage(std::max(z, -0.8f)));
            else {   
                float delta = shape[0][2] - z - ceil(shape[0][2])+1-minHeight;
                if (delta) {
                    mine.send(source, new FallMessage(-delta));
                    mine.send(source, new StopFallMessage());
                }
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
            if (!__world.isBusy(shape[0]) && 
                !__world.isBusy(shape[1]) && 
                !__world.isBusy(shape[2]) && 
                !__world.isBusy(shape[3])) {
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
        glm::vec3 position = package->position;
        glm::vec3 direction = package->direction;
        glm::vec3 delta = direction*4.f/20.f;
        bool hover = false;
        glm::ivec3 hoverPosition, placePosition;
        for (int i = 0; i<=20 && !hover; i++) {
            glm::vec3 cur = position+i*1.0f*delta;
            if (__world.isHover(cur)) {
                hover = true;
                hoverPosition = glm::ivec3(floor(cur.x), floor(cur.y), floor(cur.z));
            }
            else {
                placePosition = glm::ivec3(floor(cur.x), floor(cur.y), floor(cur.z));
            }
        }
        if (hover) {
            __world.setHoverBlock(hoverPosition, placePosition);
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
#include "World.h"
#include "Block.h"
#include "Camera.h"
#include "DrawingCenter.h"
#include "Message.h"
#include "General.h"
#include "PlayerModelController.h"
#include "PointSet.h"
#include "ShaderStorage.h"

namespace MyCraft {
    World::World(const int& x, const int& y, const int& z): __position(x-(world_side/2)*16, y-(world_side/2)*16, z-(world_side/2)*16), __isHoverBlock(false) {
        __chunkPositions.resize(world_side*world_side*world_side);
        __blockTypes = new BlockCatogary::Catogary**[16*world_side];
        __bits = new std::bitset<16*world_side>*[16*world_side];
        for (int i = 0; i<16*world_side; i++) {
            __blockTypes[i] = new BlockCatogary::Catogary*[world_side*16];
            __bits[i] = new std::bitset<16*world_side>[16*world_side];
            for (int j =0; j<16*world_side; j++) {
                __blockTypes[i][j] = new BlockCatogary::Catogary[16*world_side];
                memset(__blockTypes[i][j], BlockCatogary::Air, 16*world_side);
            }
        }
        int curr = 0;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                for (int k = 0; k<world_side; k++) {
                    __chunkPositions[curr++] = {__position.x+i*16, __position.y+j*16, __position.z+k*16, 1};
                }
            }
        }

        pFrameAlarm.setDuration(150);
        add(new CheckFallCommand(this));
        add(new CheckEmptyCommand(this));
        add(new CheckHoverCommand(this));
        add(new PlaceblockCommand(this));

    }
    World::~World() {
        for (int i = 0; i<world_side*16; i++) {
            for (int j = 0; j<world_side*16;j++) {
                delete[] __blockTypes[i][j];
            }
            delete[] __blockTypes[i];
            delete[] __bits[i];
        }
        delete[] __bits;
        delete[] __blockTypes;
    }
    
    bool World::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window);
        if (pFrameAlarm.get()) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                // if (isHovered()) {
                //     Block& pCurrentBlock = at(hX, hY, hZ);
                //     float nX = hX, nY = hY, nZ = hZ;
                //     switch (pCurrentBlock.getHoverPlane()) {
                //         case 0: nX--; break;
                //         case 1: nX++; break;
                //         case 2: nY--; break;
                //         case 3: nY++; break;
                //         case 4: nZ--; break;
                //         case 5: nZ++; break;
                //     }
                //     at(nX, nY, nZ).setType(BlockCatogary::Dirt);
                //     is_changed = true;
                // }
            }
        }
        return is_changed;
    }
    const BlockCatogary::Catogary& World::at(const int& x, const int& y, const int& z) const {
        int rX = (x-__position.x);
        int rY = (y-__position.y);
        int rZ = (z-__position.z);
        return __blockTypes[rX][rY][rZ];
    }
    const BlockCatogary::Catogary& World::at(const glm::vec3& pos) const {
        return at(std::floor(pos.x), std::floor(pos.y), std::floor(pos.z));
    }

    void World::set(const int& rx, const int& ry, const int& rz, const BlockCatogary::Catogary& type) {
        int x = (rx-__position.x);
        int y = (ry-__position.y);
        int z = (rz-__position.z);
        // pChunks[rX/16][rY/16][rZ/16].set(rX%16, rY%16, rZ%16, type);

        if (!type) {
            if (__blockTypes[x][y][z]) {
                __disableBit(x, y, z);
                __blockTypes[x][y][z] = BlockCatogary::Catogary::Air;
                if (x>0 && !__bits[x-1][y][z] && __blockTypes[x-1][y][z]) __enableBit(x-1, y, z);
                if (x<world_side*16-1 && !__bits[x+1][y][z] && __blockTypes[x+1][y][z]) __enableBit(x+1, y, z);

                if (y>0 && !__bits[x][y-1][z] && __blockTypes[x][y-1][z]) __enableBit(x, y-1, z);
                if (y<world_side*16-1 && !__bits[x][y+1][z] && __blockTypes[x][y+1][z]) __enableBit(x, y+1, z);
                
                if (z>0 && !__bits[x][y][z-1] && __blockTypes[x][y][z-1]) __enableBit(x, y, z-1);
                if (z<world_side*16-1 && !__bits[x][y][z+1] && __blockTypes[x][y][z+1]) __enableBit(x, y, z+1);
            }
        }
        else {
            if (!__blockTypes[x][y][z]) {
                __blockTypes[x][y][z] = type;
                __enableBit(x, y, z);
                if (x>0 && __bits[x-1][y][z]) __disableBit(x-1, y, z);
                if (x<world_side*16-1 && __bits[x+1][y][z]) __disableBit(x+1, y, z);

                if (y>0 && __bits[x][y-1][z]) __disableBit(x, y-1, z);
                if (y<world_side*16-1 && __bits[x][y+1][z]) __disableBit(x, y+1, z);
                
                if (z>0 && __bits[x][y][z-1]) __disableBit(x, y, z-1);
                if (z<world_side*16-1 && __bits[x][y][z+1]) __disableBit(x, y, z+1);
            }
            else {
                glm::vec4 pos(x+__position.x, y+__position.y, z+__position.z ,1);
                auto& tmp = __list[__blockTypes[x][y][z]];
                int i = 0;
                while (i<tmp.size() && tmp[i]!=pos) i++;
                if (i<tmp.size()) {
                    tmp.erase(tmp.begin()+i);
                    __blockTypes[x][y][z] = type;
                    __list[__blockTypes[x][y][z]].push_back(pos);
                }
            }
        }
    }
    
    void World::__enableBit(const int& x, const int& y, const int& z) {
        if (!__bits[x][y][z]) {
            __list[__blockTypes[x][y][z]].push_back({x+__position.x,y+__position.y,z+__position.z,1});
            __bits[x][y][z] = 1;
        }
    }
    void World::__disableBit(const int& x, const int& y, const int& z) {
        if (!__bits[x][y][z]) return;
        if (x>0 && !__blockTypes[x-1][y][z]) ;
        else if (x<world_side*16-2 && !__blockTypes[x+1][y][z]) ;
        else if (y>0 && !__blockTypes[x][y-1][z]) ;
        else if (y<world_side*16-2 && !__blockTypes[x][y+1][z]) ;
        else if (z>0 && !__blockTypes[x][y][z-1]) ;
        else if (z<world_side*16-2 && !__blockTypes[x][y][z+1]) ;
        else if (z<world_side*16-1 && z>0 && x<world_side*16-1 && x>0 && y<world_side*16-1 && y>0) {
            __bits[x][y][z] = 0;
            glm::vec4 pos(x+__position.x, y+__position.y, z+__position.z ,1);
            auto& tmp = __list[__blockTypes[x][y][z]];
            int i = 0;
            while (tmp[i]!=pos) i++;
            tmp.erase(tmp.begin()+i);
        }
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
    void World::glDraw() const {
        DrawingCenter::getInstance().BindChunk();
        DrawingCenter::getInstance().DrawChunks((void*)__chunkPositions.data(), __chunkPositions.size());
        if (__isHoverBlock) {
            DrawingCenter::getInstance().BindMargin();
            glm::vec4 vec = glm::vec4(__hoverBlock,1);
            DrawingCenter::getInstance().DrawMargin((void*)&vec, 1, 2);
        }
        DrawingCenter::getInstance().BindCube();
        for (const auto& item: __list) {
            DrawingCenter::getInstance().DrawCubes(item.first, (void*)item.second.data(), item.second.size());
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
}
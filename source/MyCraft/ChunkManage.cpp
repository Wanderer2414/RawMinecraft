#include "ChunkManage.h"
#include "Block.h"
#include "ChunkBase.h"
#include "Chunk.h"
#include "Container3D.h"
#include "DrawingCenter.h"
#include "Message.h"
#include "Sun.h"
#include "Zombie/ModelController.h"
#include "SkeletonBoss/ModelController.h"
#include "Pig/ModelController.h"
#include "ModelController.h"
#include "World.h"
#include <limits>
namespace MyCraft {
    ChunkManage::ChunkManage(const std::string& src): __isLoaded(false), __sourceFolder(src), __waterManage(*this), __time(0) {
        insert(&__waterManage);
        __chunks.resize(world_side*world_side*world_side, 0);
        __chunkPositions.resize(world_side*world_side*world_side);
        for (int i = 0; i<world_side; i++)
            for (int j = 0; j<world_side; j++) 
                for (int k = 0; k<world_side; k++) 
                    __chunkIndices[i][j][k] = i*world_side*world_side + j*world_side + k;
        __texture.load("assets/images/blockCatogary.png", false);
        __waterStillTexture.load("assets/images/WaterFlow.png", false);
        __animationClock.setDuration(100);
        __spawnClock.setDuration(1000);
        add(new TimeReceiveCommand(*this));
    }
    ChunkManage::~ChunkManage() {
        for (auto& chunk:__chunks) {
            chunk->save();
            delete chunk;
        }
    }
    void ChunkManage::setTime(const float& time) {
        __time = time;
    }
    
    bool ChunkManage::contains(const glm::ivec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= getPosition()/16;
        return (offset.x >= 0 && offset.x < world_side && offset.y >= 0 && offset.y < world_side && offset.z >= 0 && offset.z < world_side  && __chunks[__chunkIndices[offset.x][offset.y][offset.z]]);
    }
    bool ChunkManage::isDangerous(const glm::vec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= getPosition()/16;
        return (offset.x == 0 || offset.x == world_side-1 ||
                offset.y == 0 || offset.y == world_side-1 || 
                offset.z == 0 || offset.z == world_side-1);
    }
    int ChunkManage::getZHeight(const glm::vec3& position) const {
        int z = floor(position.z);
        while (contains({position.x, position.y, z}) && !isPlaceable(getType({position.x, position.y, z}))) z++;
        if (!contains({position.x, position.y, z})) z = std::numeric_limits<int>::max();
        if (!contains({position.x, position.z, z-1}) || isPlaceable(getType({position.x, position.y, z-1}))) 
            z = std::numeric_limits<int>::max();
        return z;
    }
    bool ChunkManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Container3D::handle(window);
        if (__animationClock.get()) {
            __animationClock.restart();
            for (int i = 0; i<__chunks.size(); i++)
                if (__chunks[i]) __chunks[i]->flowWater();
            is_changed = true;
        }
        for (int i = __models.size()-1; i>=0; i--) {
            is_changed = __models[i]->handle(window) || is_changed;
            if (isDangerous(__models[i]->getPosition())) {
                __dangerousModel.push_back(__models[i]);
                __models.erase(__models.begin()+i);
            }
        }
        if (__spawnClock.get() && __models.size() + __dangerousModel.size()<10) {
            glm::vec3 position(rand()%((world_side-2)*16)+16, rand()%((world_side-2)*16)+16, 2*16);
            position += getPosition();
            position.z = getZHeight(position);
            float I = getLightIndensity(position);
            if (position.z - __position.z <= world_side*16 && !isInWater(position)) {
                if (__time<0.25 || __time>0.75) {
                    if (I<50) {
                        ModelController* controller = new Zombie::Controller();
                        controller->setPosition(position);
                        send(new SpawnMobMessage(controller));
                    }
                }
                else {
                    ModelController* controller;
                    if (rand()%2) controller = new Pig::Controller();
                    else controller = new Cow::Controller();

                    controller->setPosition(position);
                    send(new SpawnMobMessage(controller));
                }
            }
        }
        return is_changed;
    }
    const std::vector<glm::vec4>& ChunkManage::getChunks() const {
        return __chunkPositions;
    }
    void ChunkManage::playerAt(const glm::ivec3& pos) {
        glm::ivec3 position(floor(pos.x/16.f) - floor(world_side/2.f), floor(pos.y/16.f) - floor(world_side/2.f), floor(pos.z/16.f) - floor(world_side/2.f));
        glm::ivec3 delta = position - __position/16;
        float length = glm::length((glm::vec3)delta);
        if (!__isLoaded || length>2) {
            __saveMobs();
            __cleanDangerous();
            __cleanSafe();
            if (!__isLoaded) __isLoaded = true;
            else {
                for (auto& chunk: __chunks) {
                    chunk->save();
                    delete chunk;
                    chunk = 0;
                }
            }
            __position = position*16;
            __loadDefault();
        }
        else if (length>=1) {
            __saveMobs();
            __cleanDangerous();
            __cleanSafe();
            if (delta.x>0) __movePositiveX();
            else if (delta.x<0) __moveNegativeX();

            if (delta.y>0) __movePositiveY();
            else if (delta.y<0) __moveNegativeY();

            if (delta.z>0) __movePositiveZ();
            else if (delta.z<0) __moveNegativeZ();
        }
    }

    void ChunkManage::pushMob(ModelController* controller) {
        if (isDangerous(controller->getPosition())) __dangerousModel.push_back(controller);
        else __models.push_back(controller);
    }
    void ChunkManage::eraseMob(ModelController* controller) {
        bool isErased = true;
        for (int i = 0; i<__models.size() && isErased; i++) 
            if (__models[i] == controller) {
                __models.erase(__models.begin() + i);
                isErased = false;
            }
        for (int i = 0; i<__dangerousModel.size() && isErased; i++) 
            if (__dangerousModel[i] == controller) {
                __dangerousModel.erase(__dangerousModel.begin() + i);
                isErased = false;
            }
    }
    void ChunkManage::__cleanSafe() {
        int size = __models.size();
        size--;
        for (int i = size; i>=0; i--) {
            if (isDangerous(__models[i]->getPosition())) {
                __dangerousModel.push_back(__models[i]);
                __models.erase(__models.begin()+i);
            }
        }
    }
    void ChunkManage::__cleanDangerous() {
        int size = __dangerousModel.size();
        size-=1; 
        for (int i = size; i>=0; i--) {
            if (!isDangerous(__dangerousModel[i]->getPosition())) {
                __models.push_back(__dangerousModel[i]);
                __dangerousModel.erase(__dangerousModel.begin()+i);
            }
        }
    }
    void ChunkManage::__saveMobs() {
        int size = __dangerousModel.size();
        size-=1;
        for (int i = size; i>=0; i--) {
            if (!contains(__dangerousModel[i]->getPosition())) {
                ModelController* tmp = __dangerousModel[i];
                send(new EraseMobMessage(__dangerousModel[i]));
                if (tmp->canSaved()) {
                    getChunk(tmp->getPosition()).pushMob(__dangerousModel[i]);
                }
                else delete tmp;
            }
        }
    }
    void ChunkManage::__loadDefault() {
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                for (int k = 0; k<world_side; k++) {
                    glm::ivec3 origin = __position/16 + glm::ivec3(i,j,k);
                    __chunks[__chunkIndices[i][j][k]] = Chunk::Load(this, __sourceFolder, origin);
                    __chunkPositions[__chunkIndices[i][j][k]] = glm::ivec4(origin*16, 16);
                }
            }
        }

        for (int i = 1; i<world_side-1; i++) {
            for (int j = 1; j<world_side-1; j++) {
                for (int k = 1; k<world_side-1; k++) {
                    __chunks[__chunkIndices[i][j][k]]->loadWater();
                }
            }
        }
    }
    void ChunkManage::__movePositiveX() {
        __position.x+=16;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[0][j][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int i = 0; i<world_side-1; i++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i+1][j][k];
                __chunkIndices[world_side-1][j][k] = index;
            }
        }
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Load new chunk
                int index = __chunkIndices[world_side-1][j][k];
                glm::ivec3  origin =  __position/16 + glm::ivec3(world_side-1, j,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int j = 1; j<world_side-1; j++) {
            for (int k = 1; k<world_side-1; k++) {
                int index = __chunkIndices[world_side-2][j][k];
                glm::ivec3  origin =  __position/16 + glm::ivec3(world_side-1, j,k);
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::__moveNegativeX() {
        __position.x-=16;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[world_side-1][j][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int i = world_side-1; i>0; i--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i-1][j][k];
                __chunkIndices[0][j][k] = index;
            }
        }
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[0][j][k];
                //Load new chunk
                glm::ivec3 origin =  __position/16 + glm::ivec3(0, j,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int j = 1; j<world_side-1; j++) {
            for (int k = 1; k<world_side-1; k++) {
                int index = __chunkIndices[1][j][k];
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::__movePositiveY() {
        __position.y+=16;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[i][0][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int j = 0; j<world_side-1; j++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j+1][k];
                __chunkIndices[i][world_side-1][k] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[i][world_side-1][k];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, world_side-1,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int i = 1; i<world_side-1; i++) {
            for (int k = 1; k<world_side-1; k++) {
                int index = __chunkIndices[i][world_side-2][k];
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::__moveNegativeY() {
        __position.y-=16;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[i][world_side-1][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;

                //Transform chunk indices table
                for (int j = world_side-1; j>0; j--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j-1][k];
                __chunkIndices[i][0][k] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[i][0][k];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, 0,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int i = 1; i<world_side-1; i++) {
            for (int k = 1; k<world_side-1; k++) {
                int index = __chunkIndices[i][1][k];
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::__movePositiveZ() {
        __position.z+=16;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                //Delete outside chunk
                int index = __chunkIndices[i][j][0];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int k = 0; k<world_side-1; k++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k+1];
                __chunkIndices[i][j][world_side-1] = index;
                
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][world_side-1];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, j,world_side-1);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int i = 1; i<world_side-1; i++) {
            for (int j = 1; j<world_side-1; j++) {
                int index = __chunkIndices[i][j][world_side-2];
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::__moveNegativeZ() {
        __position.z-=16;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][world_side-1];
                //Delete outside chunk
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int k = world_side-1; k>0; k--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k-1];
                __chunkIndices[i][j][0] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][0];;
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, j,0);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
        for (int i = 1; i<world_side-1; i++) {
            for (int j = 1; j<world_side-1; j++) {
                int index = __chunkIndices[i][j][1];
                __chunks[index]->loadWater();
            }
        }
    }
    void ChunkManage::placeDynamicWater(const glm::ivec4& position) {
        __waterManage.place(position);
    }
    void ChunkManage::glDraw() const {
        MyBase3D::Container3D::glDraw();
        for (int i = 0; i<__models.size(); i++) __models[i]->glDraw();
        for (int i = 0; i<__dangerousModel.size(); i++) __dangerousModel[i]->glDraw();
        DrawingCenter::BindCube(__texture);
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDraw();
    }
    void ChunkManage::glDrawTransparent() const {
        MyBase3D::Container3D::glDrawTransparent();
        DrawingCenter::BindCube(__texture);
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDrawTransparent();
        DrawingCenter::BindWater(__waterStillTexture);
        for (auto& chunk:__chunks) chunk->drawWater();
    }
    Chunk& ChunkManage::getChunk(const glm::ivec3& position) {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= __position/16;
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side || !__chunks[__chunkIndices[offset.x][offset.y][offset.z]])
            throw std::runtime_error("Out range of storage!");

        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const Chunk& ChunkManage::getChunk(const glm::ivec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= __position/16;
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side || !__chunks[__chunkIndices[offset.x][offset.y][offset.z]])
            throw std::runtime_error("Out range of storage!");
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const glm::ivec3& ChunkManage::getPosition() const {
        return __position;
    };

    TimeReceiveCommand::TimeReceiveCommand(ChunkManage& manage): __manage(manage) {}
    TimeReceiveCommand::~TimeReceiveCommand() {}

    MyBase::MessageType TimeReceiveCommand::getType()      const {
        return MyBase::TimeNotice;
    }
    void TimeReceiveCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        TimeMessage* package = (TimeMessage*)message;
        __manage.setTime(package->time);
    }
}
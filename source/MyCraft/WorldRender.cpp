#include "WorldRender.h"
#include "DrawingCenter.h"

namespace MyCraft {
    WorldRender::WorldRender() {
        insert(&__chunkLoader);
    }
    WorldRender::~WorldRender() {}
    void WorldRender::createPlaneWorld() const {
        __chunkLoader.create();
    }
    void WorldRender::save() {}
    void WorldRender::load(const glm::vec3& position) {
        __chunkLoader.load(position);
    }

    const BlockCatogary::Catogary& WorldRender::at(const glm::vec3& posistion) const {
        return __chunkLoader[posistion].getType(posistion);
    }

    void WorldRender::place(const glm::vec3& pos, const BlockCatogary::Catogary& type) {
        Chunk& chunk = __chunkLoader[pos];
        if (!type) {
            if (chunk.getType(pos)) {
                __disableBit(pos);
                chunk.setType(pos, BlockCatogary::Catogary::Air);
                glm::vec3 position = pos-glm::vec3(1,0,0);
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);
                position.x += 2;
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);

                position -= glm::vec3(1, 1, 0);
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);
                position += 2;
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);
                position += 2;
                if (!chunk.getBit(position) && chunk.getType(position)) __enableBit(position);
            }
        }
        else {
            if (!chunk.getType(pos)) {
                chunk.setType(pos, type);
                __enableBit(pos);
                glm::vec3 position = pos - glm::vec3(1,0,0);
                if (chunk.getBit(position)) __disableBit(position);
                position.x += 2;
                if (chunk.getBit(position)) __disableBit(position);

                position -= glm::vec3(1, 1, 0);
                if (chunk.getBit(position)) __disableBit(position);
                position.y += 2;
                if (chunk.getBit(position)) __disableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                if (chunk.getBit(position)) __disableBit(position);
                position.z += 2;
                if (chunk.getBit(position)) __disableBit(position);
            }
            else {
                chunk.disableBit(pos);
                chunk.setType(pos, type);
                chunk.enableBit(pos);
            }
        }
    }
    void WorldRender::glDraw() const {
        DrawingCenter::getInstance().BindChunk();
        DrawingCenter::getInstance().DrawChunks((void*)__chunkLoader.getChunks().data(), __chunkLoader.getChunks().size());
        Container3D::glDraw();
    }
    void WorldRender::__enableBit(const glm::vec3& pos) {
        __chunkLoader[pos].enableBit(pos);
    }
    void WorldRender::__disableBit(const glm::vec3& pos) {
        Chunk& chunk = __chunkLoader[pos];
        if (!chunk.getBit(pos)) return;
        if (!chunk.getType(pos-glm::vec3(1,0,0))) ;
        else if (!chunk.getType(pos+glm::vec3(1,0,0))) ;
        else if (!chunk.getType(pos-glm::vec3(0,1,0))) ;
        else if (!chunk.getType(pos+glm::vec3(0,1,0))) ;
        else if (!chunk.getType(pos-glm::vec3(0,0,1))) ;
        else if (!chunk.getType(pos+glm::vec3(0,0,1))) ;
        else chunk.disableBit(pos);
    }
}
#include "WorldRender.h"
#include "DrawingCenter.h"

namespace MyCraft {
    WorldRender::WorldRender(const std::string& src): __chunkLoader(src) {
        insert(&__chunkLoader);
    }
    WorldRender::~WorldRender() {}
    void WorldRender::save() {}
    void WorldRender::playerAt(const glm::vec3& position) {
        __chunkLoader.playerAt(position);
    }

    const BlockCatogary& WorldRender::at(const glm::ivec3& posistion) const {
        return __chunkLoader[posistion].getType(posistion);
    }

    void WorldRender::place(const glm::vec3& pos, const BlockCatogary& type) {
        Chunk& chunk = __chunkLoader[pos];
        if (!type) {
            if (chunk.getType(pos)) {
                chunk.disableBit(pos);
                chunk.setType(pos, BlockCatogary::Air);
                glm::vec3 position = pos;
                position.x--;
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);
                position.x += 2;
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);

                position -= glm::vec3(1, 1, 0);
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);
                position.y += 2;
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);

                position.z += 2;
                if (!__chunkLoader[position].getBit(position) && __chunkLoader[position].getType(position)) __enableBit(position);
            }
        }
        else {
            if (!chunk.getType(pos)) {
                chunk.setType(pos, type);
                __enableBit(pos);
                glm::vec3 position = pos - glm::vec3(1,0,0);
                __disableBit(position);
                position.x += 2;
                __disableBit(position);

                position -= glm::vec3(1, 1, 0);
                __disableBit(position);
                position.y += 2;
                __disableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                __disableBit(position);
                position.z += 2;
                __disableBit(position);
            }
            else {
                chunk.disableBit(pos);
                chunk.setType(pos, type);
                chunk.enableBit(pos);
            }
        }
    }
    void WorldRender::glDraw() const {
        DrawingCenter::BindMargin();
        DrawingCenter::DrawMargins((void*)__chunkLoader.getChunks().data(), __chunkLoader.getChunks().size(), RED, 2);
        Container3D::glDraw();
    }
    void WorldRender::__enableBit(const glm::vec3& pos) {
        __chunkLoader[pos].enableBit(pos);
    }
    void WorldRender::__disableBit(const glm::vec3& pos) {
        if (!__chunkLoader[pos].getBit(pos)) return;
        glm::vec3 posistion = pos;
        posistion.x -= 1;
        if (!__chunkLoader[posistion].getType(posistion)) return;
        
        posistion.x += 2;
        if (!__chunkLoader[posistion].getType(posistion)) return ;
        
        posistion -= glm::vec3(1, 1, 0);
        if (!__chunkLoader[posistion].getType(posistion)) return ;
        posistion.y += 2;
        if (!__chunkLoader[posistion].getType(posistion)) return ;

        posistion -= glm::vec3(0, 1, 1);
        if (!__chunkLoader[posistion].getType(posistion)) return ;
        posistion.z += 2;
        if (!__chunkLoader[posistion].getType(posistion)) return ;

        __chunkLoader[pos].disableBit(pos);
    }
}
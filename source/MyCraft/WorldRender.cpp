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

    Chunk& WorldRender::getChunk(const glm::ivec3& position) {
        return __chunkLoader.getChunk(position);
    };
    const Chunk& WorldRender::getChunk(const glm::ivec3& position) const {
        return __chunkLoader.getChunk(position);
    };
    const glm::ivec3& WorldRender::getPosition() const {
        return __chunkLoader.getPosition();
    }
    void WorldRender::glDraw() const {
        DrawingCenter::BindMargin();
        DrawingCenter::DrawMargins((void*)__chunkLoader.getChunks().data(), __chunkLoader.getChunks().size(), RED, 2);
        Container3D::glDraw();
    }
}
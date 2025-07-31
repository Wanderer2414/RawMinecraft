#ifndef WORLD_DATA_H
#define WORLD_DATA_H
#include "Block.h"
#include "ChunkLoader.h"
#include "Container3D.h"
#include "Controller3D.h"

namespace MyCraft {
    class WorldRender: public MyBase3D::Container3D, public ChunkLoader {
    public:
        WorldRender(const std::string& src);
        ~WorldRender();
        void save();
        void playerAt(const glm::vec3& position);
        
    protected:
        void glDraw() const override;
        Chunk& getChunk(const glm::ivec3& position) override;
        const Chunk& getChunk(const glm::ivec3& position) const override;
        const glm::ivec3& getPosition() const override;
    private:
        ChunkManage __chunkLoader;
    };
}
#endif
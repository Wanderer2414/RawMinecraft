#ifndef CHUNK_MANAGE_H
#define CHUNK_MANAGE_H
#include "Block.h"
#include "ChunkBase.h"
#include "Clock.h"
#include "Container3D.h"
#include "Texture.h"
#include "WaterManage.h"

namespace MyCraft {
    class ChunkManage: public MyBase3D::Container3D, public ChunkLoader {
    public:
        ChunkManage(const std::string& src);
        ~ChunkManage();
        bool contains(const glm::ivec3& position) const override;
        const std::vector<glm::vec4>& getChunks() const;
        void playerAt(const glm::ivec3& position);
        Chunk&          getChunk(const glm::ivec3&)         override;
        const Chunk&    getChunk(const glm::ivec3&) const   override;
        void placeDynamicWater(const glm::ivec4& position) override;
        const glm::ivec3& getPosition() const override;
    protected:
    private:
        #define world_side 7
        bool                    __isLoaded;
        glm::ivec3              __position;
        std::vector<glm::vec4>  __chunkPositions;
        std::vector<Chunk*>     __chunks;
        std::queue<Chunk*>      __storageQueue;
        int                     __chunkIndices[world_side][world_side][world_side];
        std::string             __sourceFolder;
        MyBase::Texture         __texture, __waterStillTexture;
        WaterManage             __waterManage;
        MyBase::Clock           __animationClock;
        void __loadDefault();
        void __movePositiveX(), __moveNegativeX();
        void __movePositiveY(), __moveNegativeY();
        void __movePositiveZ(), __moveNegativeZ();
        bool handle(GLFWwindow* window) override;
        void glDraw() const override;
        void glDrawTransparent() const override;
        void __loadWater(Chunk* chunk);

    };
}

#endif
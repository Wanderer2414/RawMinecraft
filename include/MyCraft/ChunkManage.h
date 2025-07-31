#ifndef CHUNK_MANAGE_H
#define CHUNK_MANAGE_H
#include "ChunkBase.h"
#include "Container3D.h"

namespace MyCraft {
    class ChunkManage: public MyBase3D::Container3D, public ChunkLoader {
    public:
        ChunkManage(const std::string& src);
        ~ChunkManage();
        const std::vector<glm::ivec4>& getChunks() const;
        void playerAt(const glm::ivec3& position);
        Chunk&          getChunk(const glm::ivec3&)         override;
        const Chunk&    getChunk(const glm::ivec3&) const   override;
        const glm::ivec3& getPosition() const override;
    protected:
    private:
        #define world_side 7
        bool                    __isLoaded;
        glm::ivec3              __position;
        std::vector<glm::ivec4> __chunkPositions;
        std::vector<Chunk*>     __chunks;
        std::queue<Chunk*>      __storageQueue;
        int                     __chunkIndices[world_side][world_side][world_side];
        std::string             __sourceFolder;
        void __loadDefault();
        static void __moveSubPositiveX(ChunkManage*, const int& y, const int& z);
        static void __moveSubNegativeX(ChunkManage*, const int& y, const int& z);
        void __movePositiveX(), __moveNegativeX();
        static void __moveSubPositiveY(ChunkManage*, const int& x, const int& z);
        static void __moveSubNegativeY(ChunkManage*, const int& x, const int& z);
        void __movePositiveY(), __moveNegativeY();
        static void __moveSubPositiveZ(ChunkManage*, const int& x, const int& z);
        static void __moveSubNegativeZ(ChunkManage*, const int& x, const int& z);
        void __movePositiveZ(), __moveNegativeZ();
        void glDraw() const override;
        void glDrawTransparent() const override;

    };
}

#endif
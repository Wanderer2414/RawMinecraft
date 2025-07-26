#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H
#include "Chunk.h"
#include "Container3D.h"
#include "Controller3D.h"

namespace MyCraft {
    class ChunkLoader: public MyBase3D::Container3D {
    public:
        ChunkLoader(const std::string& src);
        ~ChunkLoader();
        const std::vector<glm::vec4>& getChunks() const;
        void playerAt(const glm::vec3& position);

        Chunk& operator[](const glm::vec3& posistion);
        const Chunk& operator[](const glm::vec3& posistion) const;
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
        void __loadDefault();
        static void __moveSubPositiveX(ChunkLoader*, const int& y, const int& z);
        static void __moveSubNegativeX(ChunkLoader*, const int& y, const int& z);
        void __movePositiveX(), __moveNegativeX();
        static void __moveSubPositiveY(ChunkLoader*, const int& x, const int& z);
        static void __moveSubNegativeY(ChunkLoader*, const int& x, const int& z);
        void __movePositiveY(), __moveNegativeY();
        static void __moveSubPositiveZ(ChunkLoader*, const int& x, const int& z);
        static void __moveSubNegativeZ(ChunkLoader*, const int& x, const int& z);
        void __movePositiveZ(), __moveNegativeZ();
        void glDraw() const override;

    };
}
#endif
#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H
#include "Chunk.h"
#include "Container3D.h"
#include "Controller3D.h"
#include "General.h"
namespace MyCraft {
    class ChunkLoader: public MyBase3D::Container3D {
    public:
        ChunkLoader();
        ~ChunkLoader();
        const std::vector<glm::vec4>& getChunks() const;
        void create() const;
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
        std::string getFileName(const glm::ivec3& position) const;
        void __loadDefault();
        void glDraw() const override;

    };
}
#endif
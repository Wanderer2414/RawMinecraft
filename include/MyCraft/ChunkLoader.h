#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H
#include "Chunk.h"
#include "Controller3D.h"
namespace MyCraft {
    class ChunkLoader: public MyBase3D::Controller3D {
    public:
        ChunkLoader();
        ~ChunkLoader();
        const std::vector<glm::vec4>& getChunks() const;
        void create() const;
        void load(const glm::vec3& position);

        Chunk& operator[](const glm::vec3& posistion);
        const Chunk& operator[](const glm::vec3& posistion) const;
    protected:
    private:
        glm::vec3 __position;
        void glDraw() const override;
        std::vector<glm::vec4> __chunkPositions;
        std::vector<Chunk*> __chunks;
        int                 __chunkIndices[5][5][5];
        std::string getFileName(const glm::vec3& position) const;
    };
}
#endif
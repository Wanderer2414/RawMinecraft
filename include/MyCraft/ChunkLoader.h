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
        static void create(const std::string& src);
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
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        void __loadDefault();
        void __movePositiveX(), __moveNegativeX();
        void __movePositiveY(), __moveNegativeY();
        void __movePositiveZ(), __moveNegativeZ();
        void glDraw() const override;

    };
}
#endif
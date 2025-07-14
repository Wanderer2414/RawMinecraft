#ifndef WORLD_DATA_H
#define WORLD_DATA_H
#include "Block.h"
#include "ChunkLoader.h"
#include "Container3D.h"
#include "Controller3D.h"

namespace MyCraft {
    class WorldRender: public MyBase3D::Container3D {
    public:
        WorldRender();
        ~WorldRender();
        void createPlaneWorld() const;
        void save();
        void load(const glm::vec3& position);

        BlockCatogary::Catogary&            at(const glm::vec3& posistion);
        const BlockCatogary::Catogary&      at(const glm::vec3& posistion) const;

        void place(const glm::vec3& pos, const BlockCatogary::Catogary& type);
    protected:
        void glDraw() const override;
    private:
        glm::vec3 __position;
        void __enableBit(const glm::vec3& pos);
        void __disableBit(const glm::vec3& pos);
        ChunkLoader __chunkLoader;
    };
}
#endif
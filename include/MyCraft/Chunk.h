#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Controller3D.h"
namespace MyCraft {
    class Chunk: public MyBase3D::Controller3D {
    public:
        Chunk();
        ~Chunk();
        BlockCatogary::Catogary& getType(const glm::vec3& pos) const;
        void save(const std::string& file);
        void setType(const glm::vec3& pos, const BlockCatogary::Catogary& type);
        std::bitset<16>::reference getBit(const glm::vec3& pos);
        void enableBit(const glm::vec3& pos);
        void disableBit(const glm::vec3& pos);
        static Chunk* Load(const std::string& file);
    protected:
    private:
        void glDraw() const override;
        std::map<BlockCatogary::Catogary, std::vector<glm::vec4>>  __list;
        glm::vec3                   __position;
        BlockCatogary::Catogary     ***__blockTypes;
        std::bitset<16>             **__bits;
        std::vector<std::vector<std::vector<int>>> __tableIndexes;
    };
}
#endif
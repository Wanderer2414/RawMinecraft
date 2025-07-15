#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Controller3D.h"
namespace MyCraft {
    class Chunk {
    public:
        ~Chunk();
        const BlockCatogary::Catogary& getType(const glm::vec3& pos) const;
        void save();
        void setType(const glm::vec3& pos, const BlockCatogary::Catogary& type);
        std::bitset<16>::reference getBit(const glm::vec3& pos);
        void enableBit(const glm::vec3& pos);
        void disableBit(const glm::vec3& pos);
        glm::vec3 getPosition() const;
        static Chunk* Load(const std::string& file);
        void glDraw() const;
    protected:
    private:
        Chunk();
        bool    __isChange;
        std::map<BlockCatogary::Catogary, std::vector<glm::vec4>>  __list;
        glm::vec3                   __position;
        BlockCatogary::Catogary     __blockTypes[16][16][16];
        std::bitset<16>             __bits[16][16];
        int                         __tableIndexes[16][16][16];
        std::string                 __source;
    };
}
#endif
#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Controller3D.h"
namespace MyCraft {
    class Chunk {
    public:
        ~Chunk();
        const BlockCatogary& getType(const glm::ivec3& pos) const;
        void save();
    void setType(const glm::ivec3& pos, const BlockCatogary& type);
        std::bitset<16>::reference getBit(const glm::ivec3& pos);
        void enableBit(const glm::ivec3& pos);
        void disableBit(const glm::ivec3& pos);
        glm::ivec3 getPosition() const;
        static Chunk* Load(const std::string& src, const glm::ivec3& position);
        void glDraw() const;
        friend class MapCreator;
    protected:
    private:
        Chunk();
        bool                        __isChange;
        unsigned int                __numBlock, __numBit;
        int                         __tableIndexes[16][16][16];
        glm::ivec3                  __position;
        BlockCatogary               __blockTypes[16][16][16];
        std::bitset<16>             __bits[16][16];
        std::string                 __source;
        std::vector<glm::ivec4>      __list;
    };
}
#endif
#ifndef CHUNK_H
#define CHUNK_H
#include "Block.h"
#include "Controller3D.h"
namespace MyCraft {
    class Chunk {
    public:
        ~Chunk();
        const BlockCatogary& getType(const glm::ivec3& pos) const;
        const BlockCatogary& getLocalType(const glm::ivec3& pos) const;
        void save();
        void disableList();
        void enableList();
        void setType(const glm::ivec3& pos, const BlockCatogary& type);
        void setLocalType(const glm::ivec3& pos, const BlockCatogary& type);
        std::bitset<16>::reference getBit(const glm::ivec3& pos);
        std::bitset<16>::reference getLocalBit(const glm::ivec3& offset);
        void enableBit(const glm::ivec3& pos);
        void enableLocalBit(const glm::ivec3& pos);
        void disableLocalBit(const glm::ivec3& posos);
        void disableBit(const glm::ivec3& pos);
        glm::ivec3 getPosition() const;
        static Chunk* Load(const std::string& src, const glm::ivec3& position);
        void glDraw() const;
    protected:
    private:
        Chunk();
        bool                        __isChange, __enableQueue;
        unsigned int                __numBlock, __numBit;
        int                         __tableIndexes[16][16][16];
        glm::ivec3                  __position;
        BlockCatogary               __blockTypes[16][16][16];
        std::bitset<16>             __bits[16][16];
        std::string                 __source;
        std::vector<glm::ivec4>      __list;
    };

    class DynamicChunk {
    public:
        DynamicChunk(const std::string& src);
        ~DynamicChunk();
        const BlockCatogary& getType(const glm::ivec3& pos);
        const BlockCatogary& getLocalType(const glm::ivec3& pos);
        void save();
        void disableList();
        void enableList();
        void place(const glm::ivec3& pos, const BlockCatogary& type);
        void setType(const glm::ivec3& pos, const BlockCatogary& type);
        void setLocalType(const glm::ivec3& pos, const BlockCatogary& type);
        void Load(const glm::ivec3& position);
        std::bitset<16>::reference getBit(const glm::ivec3& pos);
        std::bitset<16>::reference getLocalBit(const glm::ivec3& offset);
        void enableBit(const glm::ivec3& pos);
        void enableLocalBit(const glm::ivec3& pos);
        void disableLocalBit(const glm::ivec3& posos);
        void disableBit(const glm::ivec3& pos);
        glm::ivec3 getPosition() const;
        void glDraw() const;
    protected:
    private:
        std::string __source;
        Chunk* __chunk;

        void __disableBit(const glm::ivec3& position);
    };
}
#endif
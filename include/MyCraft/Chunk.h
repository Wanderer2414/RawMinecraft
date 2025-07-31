#ifndef CHUNK_H
#define CHUNK_H
#include "ChunkBase.h"

namespace MyCraft {

    class Chunk: public ChunkObject {
    public:
        ~Chunk();
        void save();
        void disableList();
        void enableList();
        void setType(const glm::ivec3& pos, const BlockCatogary& type)          override;
        static Chunk* Load(const std::string& src, const glm::ivec3& position);
        void glDraw() const;
        void glDrawTransparent() const;
        const BlockCatogary&                getType(const glm::ivec3& pos) const   override;
        const BlockCatogary&                getType(const glm::ivec3& pos)      override;
        std::bitset<16>::reference          getBit(const glm::ivec3& pos)       override;
        void enableBit(const glm::ivec3& position)                                   override;
        void disableBit(const glm::ivec3& position)                                  override;

        const glm::ivec3& getPosition() const                                   override;
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
        std::vector<glm::ivec4>     __list;
        std::vector<glm::ivec4>     __transparentList;
        void __add(const glm::ivec3& position);
        void __remove(const glm::ivec3& position);
    };


    class DynamicChunk: public ChunkLoader {
    public:
        DynamicChunk(const std::string& src);
        ~DynamicChunk();
        void save();
        void disableList();
        void enableList();
        
        const glm::ivec3& getPosition() const           override;
        void glDraw() const;
    protected:
        Chunk&          getChunk(const glm::ivec3&) override;
        const Chunk&    getChunk(const glm::ivec3&) const override;
    private:
        std::string __source;
        Chunk* __chunk;
    };
}
#endif
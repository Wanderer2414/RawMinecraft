#ifndef CHUNK_BASE_H
#define CHUNK_BASE_H
#include "Block.h"
#include "Container3D.h"
#include "Controller3D.h"

namespace MyCraft {
    class Chunk;
    class ChunkObject {
    public:
        ChunkObject()                   = default;
        ChunkObject(const ChunkObject&) = delete;
        ChunkObject(ChunkObject&&)      = delete;
        ChunkObject& operator=(const ChunkObject&) const = delete; 

        virtual const BlockCatogary&    getType(const glm::ivec3&) const        = 0;
        virtual const BlockCatogary&    getType(const glm::ivec3&)              = 0;
        virtual void setType(const glm::ivec3&, const BlockCatogary& type)      = 0;

        const BlockCatogary&            getLocalType(const glm::ivec3&) const   ;
        const BlockCatogary&            getLocalType(const glm::ivec3&)         ;
        void                            setLocalType(const glm::ivec3&, const BlockCatogary& type) ;
        void                            enableLocalBit(const glm::ivec3&)       ;
        void                            disableLocalBit(const glm::ivec3&)      ;
        virtual std::bitset<16>::reference          getLocalBit(const glm::ivec3&);

        virtual std::bitset<16>::reference          getBit(const glm::ivec3&)       = 0;
        virtual void                                enableBit(const glm::ivec3&)    = 0;
        virtual void                                disableBit(const glm::ivec3&)   = 0;
    protected:

        virtual const glm::ivec3& getPosition() const                           = 0;
    private:
    };
    class ChunkLoader: public ChunkObject {
    public:
        ChunkLoader()                   = default;
        ChunkLoader(const ChunkLoader&) = delete;
        ChunkLoader& operator=(const ChunkLoader&) const = delete; 

        const BlockCatogary&    getType(const glm::ivec3&) const   override;
        const BlockCatogary&    getType(const glm::ivec3&)         override;
        void setType(const glm::ivec3&, const BlockCatogary& type) override;
        std::bitset<16>::reference  getBit(const glm::ivec3&)     override;
        void                    enableBit(const glm::ivec3&)       override;
        void                    disableBit(const glm::ivec3&)      override;
        virtual Chunk&          getChunk(const glm::ivec3&)         = 0;
        virtual const Chunk&    getChunk(const glm::ivec3&) const   = 0;
    protected:
    private:
    };
    
}
#endif
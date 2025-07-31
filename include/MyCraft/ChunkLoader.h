#ifndef CHUNK_LOADER_H
#define CHUNK_LOADER_H
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

    class Chunk: public ChunkObject {
    public:
        ~Chunk();
        void save();
        void disableList();
        void enableList();
        void setType(const glm::ivec3& pos, const BlockCatogary& type)          override;
        static Chunk* Load(const std::string& src, const glm::ivec3& position);
        void glDraw() const;
        const BlockCatogary&                getType(const glm::ivec3& pos) const   override;
        const BlockCatogary&                getType(const glm::ivec3& pos)      override;
        std::bitset<16>::reference          getBit(const glm::ivec3& pos)       override;
        void enableBit(const glm::ivec3& pos)                                   override;
        void disableBit(const glm::ivec3& pos)                                  override;

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
        std::vector<glm::ivec4>      __list;
    };

    class ChunkManage: public MyBase3D::Container3D, public ChunkLoader {
    public:
        ChunkManage(const std::string& src);
        ~ChunkManage();
        const std::vector<glm::ivec4>& getChunks() const;
        void playerAt(const glm::ivec3& position);
        Chunk&          getChunk(const glm::ivec3&)         override;
        const Chunk&    getChunk(const glm::ivec3&) const   override;
        const glm::ivec3& getPosition() const override;
    protected:
    private:
        #define world_side 7
        bool                    __isLoaded;
        glm::ivec3              __position;
        std::vector<glm::ivec4> __chunkPositions;
        std::vector<Chunk*>     __chunks;
        std::queue<Chunk*>      __storageQueue;
        int                     __chunkIndices[world_side][world_side][world_side];
        std::string             __sourceFolder;
        void __loadDefault();
        static void __moveSubPositiveX(ChunkManage*, const int& y, const int& z);
        static void __moveSubNegativeX(ChunkManage*, const int& y, const int& z);
        void __movePositiveX(), __moveNegativeX();
        static void __moveSubPositiveY(ChunkManage*, const int& x, const int& z);
        static void __moveSubNegativeY(ChunkManage*, const int& x, const int& z);
        void __movePositiveY(), __moveNegativeY();
        static void __moveSubPositiveZ(ChunkManage*, const int& x, const int& z);
        static void __moveSubNegativeZ(ChunkManage*, const int& x, const int& z);
        void __movePositiveZ(), __moveNegativeZ();
        void glDraw() const override;

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
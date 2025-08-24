#ifndef CHUNK_H
#define CHUNK_H
#include "ChunkBase.h"
#include "DrawingCenter.h"
#include "ModelController.h"
#include <unordered_set>

namespace MyCraft {

    class Chunk: public ChunkObject {
    public:
        ~Chunk();
        bool isInWater(const glm::ivec3& position) const override;
        void save();
        void disableList();
        void enableList();
        void setType(const glm::ivec3& pos, const BlockCatogary& type)          override;
        void setState(const glm::ivec3& pos, const glm::mat4& state)            override;
        void setLight(const glm::ivec3& position, const float& indensity)       override;
        bool pourWater(const glm::ivec3& position, const glm::vec4& height)      override;
        bool takeWater(const glm::ivec3& position)                              override;
        void enableWaterPlane(const glm::ivec3& position, const unsigned char& plane);
        void disableWaterPlane(const glm::ivec3& position, const unsigned char& plane);
        float getWaterHeight(const glm::ivec3& position) const                  override;
        glm::mat4 getState(const glm::ivec3&) const                             override;
        static Chunk* Load(ChunkLoader* loader, const std::string& src, const glm::ivec3& position);
        void glDraw() const;
        void glDrawTransparent() const;
        const BlockCatogary&                getType(const glm::ivec3& pos) const   override;
        const BlockCatogary&                getType(const glm::ivec3& pos)      override;
        std::bitset<16>::reference          getBit(const glm::ivec3& pos)       override;
        void enableBit(const glm::ivec3& position)                              override;
        void disableBit(const glm::ivec3& position)                             override;
        void drawWater() const;
        void flowWater();
        void loadWater();
        void pushDynamicWater(const glm::ivec4& position)                       override;
        void popDynamicWater(const glm::ivec4& position)                        override;
        const glm::ivec3& getPosition() const                                   override;
    protected:
    private:
        Chunk();
        bool                        __isChange, __enableQueue;
        unsigned int                __numBlock, __numBit;
        int                         __tableIndexes[16][16][16];
        glm::ivec3                  __position;
        BlockCatogary               __blockTypes[16][16][16];
        unsigned char               __waterHeight[16][16][16];
        std::bitset<16>             __bits[16][16];
        std::string                 __source;
        BlockDrawingStorage         __normal, __transparent;
        WaterDrawingStorage         __water;
        std::unordered_set<int>     __lightSource;
        std::map<unsigned int, glm::mat4>   __specialState;
        std::queue<glm::ivec4>      __waterFlowing;
        ChunkLoader                 *__container;
        std::vector<ModelController*> __savableModel;
        std::vector<ModelController*> __unsavableModel;
        void __add_block(const glm::ivec3& position);
        void __remove_block(const glm::ivec3& position);
    };


    class DynamicChunk: public ChunkLoader {
    public:
        DynamicChunk(const std::string& src);
        ~DynamicChunk();
        bool contains(const glm::ivec3&) const override;
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
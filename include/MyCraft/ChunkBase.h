#ifndef CHUNK_BASE_H
#define CHUNK_BASE_H
#include "Block.h"
#include "LightDistribution.h"

namespace MyCraft {
    class Chunk;
    class ChunkObject {
    public:
        ChunkObject()                   = default;
        ChunkObject(const ChunkObject&) = delete;
        ChunkObject(ChunkObject&&)      = delete;
        ChunkObject& operator=(const ChunkObject&) const = delete; 
        virtual ~ChunkObject();
        virtual bool isInWater(const glm::ivec3& position) const = 0;
        virtual const BlockCatogary&    getType(const glm::ivec3&) const        = 0;
        virtual const BlockCatogary&    getType(const glm::ivec3&)              = 0;
        virtual void setType(const glm::ivec3&, const BlockCatogary& type)      = 0;
        virtual void setState(const glm::ivec3& position, const glm::mat4& state) = 0;
        virtual void setLight(const glm::ivec3& position, const float& indensity) = 0;
        virtual bool pourWater(const glm::ivec3& position, const glm::vec4& height)  = 0;
        virtual float getWaterHeight(const glm::ivec3& position) const          = 0;
        virtual glm::mat4 getState(const glm::ivec3&) const = 0;
        const BlockCatogary&            getLocalType(const glm::ivec3&) const   ;
        const BlockCatogary&            getLocalType(const glm::ivec3&)         ;
        void                            setLocalType(const glm::ivec3&, const BlockCatogary& type) ;
        void                            setLocalState(const glm::ivec3& position, const glm::mat4& state);
        void                            enableLocalBit(const glm::ivec3&)       ;
        void                            disableLocalBit(const glm::ivec3&)      ;
        void                            setLocalLight(const glm::ivec3& position, const float& indensity);
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
        virtual bool isInWater(const glm::ivec3& position) const override;
        virtual bool contains(const glm::ivec3& position) const = 0;
        const BlockCatogary&    getType(const glm::ivec3&) const   override;
        const BlockCatogary&    getType(const glm::ivec3&)         override;
        void setType(const glm::ivec3&, const BlockCatogary& type) override;
        void setState(const glm::ivec3& pos, const glm::mat4& state) override;
        bool pourWater(const glm::ivec3& position, const glm::vec4& height)  override;
        
        float getWaterHeight(const glm::ivec3& position) const          override;
        glm::mat4 getState(const glm::ivec3&) const                 override;
        std::bitset<16>::reference  getBit(const glm::ivec3&)     override;
        void                    enableBit(const glm::ivec3&)       override;
        void                    disableBit(const glm::ivec3&)      override;
        virtual void            pushDynamicWater(const glm::ivec3& position);
        void setLight(const glm::ivec3& position, const float& indensity) override;
        void removeLight(const glm::ivec3& position);
        virtual Chunk&          getChunk(const glm::ivec3&)         = 0;
        virtual const Chunk&    getChunk(const glm::ivec3&) const   = 0;
        float getLightIndensity(const glm::ivec3& position) const;
    protected:
    private:
        BvhLightTree    tree;
    };
    
}
#endif
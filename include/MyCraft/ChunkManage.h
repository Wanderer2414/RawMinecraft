#ifndef CHUNK_MANAGE_H
#define CHUNK_MANAGE_H
#include "Block.h"
#include "ChunkBase.h"
#include "Clock.h"
#include "Container3D.h"
#include "Message.h"
#include "ModelController.h"
#include "Texture.h"
#include "WaterManage.h"

namespace MyCraft {
    class ChunkManage: public MyBase3D::Container3D, public ChunkLoader, public MyBase::Port {
    public:
        ChunkManage(const std::string& src);
        ~ChunkManage();
        int             getZHeight(const glm::vec3& position) const;
        bool            contains(const glm::ivec3& position) const override;
        bool            isDangerous(const glm::vec3& position) const;
        const std::vector<glm::vec4>& getChunks() const;
        void            playerAt(const glm::ivec3& position);
        Chunk&          getChunk(const glm::ivec3&)         override;
        const Chunk&    getChunk(const glm::ivec3&) const   override;
        void            placeDynamicWater(const glm::ivec4& position) override;
        const glm::ivec3& getPosition() const override;

        void            pushMob(ModelController* controller);
        void            eraseMob(ModelController* controller);

        void setTime(const float& time);
    protected:
    private:
        #define world_side 7
        bool                    __isLoaded;
        float                   __time;
        glm::ivec3              __position;
        std::vector<glm::vec4>  __chunkPositions;
        std::vector<Chunk*>     __chunks;
        std::queue<Chunk*>      __storageQueue;
        int                     __chunkIndices[world_side][world_side][world_side];
        std::string             __sourceFolder;
        MyBase::Texture         __texture, __waterStillTexture;
        WaterManage             __waterManage;
        MyBase::Clock           __animationClock, __spawnClock;
        void __loadDefault();
        void __movePositiveX(), __moveNegativeX();
        void __movePositiveY(), __moveNegativeY();
        void __movePositiveZ(), __moveNegativeZ();
        bool handle(GLFWwindow* window) override;
        void glDraw() const override;
        void glDrawTransparent() const override;
        void __loadWater(Chunk* chunk);
        void __cleanSafe();
        void __cleanDangerous();
        void __saveMobs();
        std::vector<ModelController*> __models, __dangerousModel;
    };
    class TimeReceiveCommand: public MyBase::Command {
    public:
        TimeReceiveCommand(ChunkManage& manage);
        ~TimeReceiveCommand();

        MyBase::MessageType getType()      const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        ChunkManage& __manage;
    };
}

#endif
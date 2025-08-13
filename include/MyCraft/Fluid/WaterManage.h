#ifndef WATER_MANAGE_H
#define WATER_MANAGE_H
#include "ChunkBase.h"
#include "Clock.h"
#include "Controller3D.h"
#include "DrawingCenter.h"
#include "Texture.h"
namespace MyCraft {
    class ChunkManage;
    class WaterManage: public MyBase3D::Controller3D {
    public:
        WaterManage(ChunkManage& WorldRender);
        ~WaterManage();
        WaterManage(const WaterManage&) = delete;
        WaterManage& operator=(const WaterManage&) const = delete; 
        void place(const glm::ivec3& position);
    protected:
    private:
        MyBase::Texture     __waterTexture;
        ChunkManage&        __manage;
        MyBase::Clock       __flowClock;
        WaterDrawingStorage __water;
        std::queue<glm::ivec3> __waterFlow;
        bool handle(GLFWwindow* window) override;
        void glDrawTransparent() const override;
        void __push(const glm::ivec3& position, const float& height);
    };
}
#endif
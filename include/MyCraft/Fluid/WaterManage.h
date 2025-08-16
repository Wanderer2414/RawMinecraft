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
        void place(const glm::ivec4& position);
    protected:
    private:
        ChunkManage&        __manage;
        MyBase::Clock       __flowClock;
        std::queue<glm::ivec4> __waterFlow;
        bool handle(GLFWwindow* window) override;
    };
}
#endif
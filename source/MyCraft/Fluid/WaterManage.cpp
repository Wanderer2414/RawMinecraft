#include "WaterManage.h"
#include "Block.h"
#include "ChunkManage.h"
#include "DrawingCenter.h"
namespace MyCraft {
    
    WaterManage::WaterManage(ChunkManage& WorldRender): __manage(WorldRender) {
        __waterTexture.load("assets/images/Water.png", false);
        __flowClock.setDuration(1000);
    }
    WaterManage::~WaterManage() {}
    void WaterManage::place(const glm::ivec3& position) {
        if (__manage.getWaterHeight(position)<1) {
            __manage.setWater(position, 1);
            __water.push(position, glm::vec4(1), __manage.getLightIndensity(position)*5.f/255);
            __waterFlow.push(position);
        }
    }
    bool WaterManage::handle(GLFWwindow* window) {
        if (__flowClock.get()) {
            __flowClock.restart();
            int size = __waterFlow.size();
            for (int i = 0; i<size; i++) {
                glm::ivec3 position = __waterFlow.front();
                __waterFlow.pop();
                float height = __manage.getWaterHeight(position);
                if (height) {
                    glm::ivec3 cur = position+glm::ivec3(1,0,0);
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, height-0.2);
                        __push(cur, height-0.2);
                        __waterFlow.push(cur);
                    }
                    cur.x -= 2;
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, height-0.2);
                        __push(cur, height-0.2);
                        __waterFlow.push(cur);
                    }

                    cur += glm::ivec3(1, 1, 0);
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, height-0.2);
                        __push(cur, height-0.2);
                        __waterFlow.push(cur);
                    }

                    cur.y -= 2;
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, height-0.2);
                        __push(cur, height-0.2);
                        __waterFlow.push(cur);
                    }
                }
                else {
                    glm::ivec3 cur = position+glm::ivec3(1,0,0);
                    if (!__manage.getWaterHeight(cur)) {
                        __push(cur, 0);
                    }
                    cur.x -= 2;
                    if (!__manage.getWaterHeight(cur)) {
                        __push(cur, 0);
                    }

                    cur += glm::ivec3(1, 1, 0);
                    if (!__manage.getWaterHeight(cur)) {
                        __push(cur, 0);
                    }

                    cur.y -= 2;
                    if (!__manage.getWaterHeight(cur)) {
                        __push(cur, 0);
                    }
                }
            }
        }
        return false;
    }
    void WaterManage::__push(const glm::ivec3& position, const float& height) {
        __manage.setWater(position, height);
        glm::vec4 heights(height);
        heights[0] = std::max(heights[0], __manage.getWaterHeight(position-glm::ivec3(1,0,0)));
        heights[0] = std::max(heights[0], __manage.getWaterHeight(position-glm::ivec3(0,1,0)));
        heights[0] = std::max(heights[0], __manage.getWaterHeight(position-glm::ivec3(1,1,0)));

        heights[1] = std::max(heights[1], __manage.getWaterHeight(position+glm::ivec3(1,0,0)));
        heights[1] = std::max(heights[1], __manage.getWaterHeight(position-glm::ivec3(0,1,0)));
        heights[1] = std::max(heights[1], __manage.getWaterHeight(position+glm::ivec3(1,-1,0)));

        heights[2] = std::max(heights[2], __manage.getWaterHeight(position+glm::ivec3(1,0,0)));
        heights[2] = std::max(heights[2], __manage.getWaterHeight(position+glm::ivec3(0,1,0)));
        heights[2] = std::max(heights[2], __manage.getWaterHeight(position+glm::ivec3(1,1,0)));

        heights[3] = std::max(heights[3], __manage.getWaterHeight(position-glm::ivec3(1,0,0)));
        heights[3] = std::max(heights[3], __manage.getWaterHeight(position+glm::ivec3(0,1,0)));
        heights[3] = std::max(heights[3], __manage.getWaterHeight(position+glm::ivec3(-1,1,0)));
        if (glm::length(heights)) __water.push(position, heights, __manage.getLightIndensity(position)*5.f/255);
    }
    void WaterManage::glDrawTransparent() const {
        DrawingCenter::BindWater(__waterTexture);
        DrawingCenter::DrawWater(__water);
    }
}
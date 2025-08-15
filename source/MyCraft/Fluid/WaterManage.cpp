#include "WaterManage.h"
#include "Block.h"
#include "ChunkManage.h"

namespace MyCraft {
    
    WaterManage::WaterManage(ChunkManage& WorldRender): __manage(WorldRender) {
        __flowClock.setDuration(500);
    }
    WaterManage::~WaterManage() {}
    void WaterManage::place(const glm::ivec4& position) {
        if (__manage.getWaterHeight(position)<1) {
            __manage.pourWater(position, glm::vec4(position.w/10.f));
            __waterFlow.push(position);
            __manage.pushDynamicWater(position);
            __flowClock.restart();
        }
    }
    bool WaterManage::handle(GLFWwindow* window) {
        if (__flowClock.get() && __waterFlow.size()) {
            __flowClock.restart();
            int size = __waterFlow.size();
            for (int i = 0; i<size; i++) {
                glm::ivec4 position = __waterFlow.front();
                __waterFlow.pop();
                if (__manage.contains(position)) {
                    __manage.popDynamicWater(position);
                    float height = std::max(position.w/10.f, __manage.getWaterHeight(position))- 0.2f;
                    if (isCollistion(__manage.getType(position-glm::ivec4(0,0,1, 0)))) {
                        glm::ivec4 cur = position+glm::ivec4(1,0,0, -2);
                        if (__manage.pourWater(cur, glm::vec4(height))) {
                            __waterFlow.push(cur);
                            __manage.pushDynamicWater(cur);
                        }
                        cur.x -= 2;
                        if (__manage.pourWater(cur, glm::vec4(height))) {
                            __waterFlow.push(cur);
                            __manage.pushDynamicWater(cur);
                        }
                        

                        cur += glm::ivec4(1, 1, 0, 0);
                        if (__manage.pourWater(cur, glm::vec4(height))) {
                            __waterFlow.push(cur);
                            __manage.pushDynamicWater(cur);
                        }
                        cur.y -= 2;
                        if (__manage.pourWater(cur, glm::vec4(height))) {
                            __waterFlow.push(cur);
                            __manage.pushDynamicWater(cur);
                        }
                    }
                    else {
                        if (__manage.pourWater(position-glm::ivec4(0,0,1,0), glm::vec4(1))) {
                            __waterFlow.push(position-glm::ivec4(0,0,1,2));
                            __manage.pushDynamicWater(position-glm::ivec4(0,0,1,2));
                        }
                    }
                }
            }
        }
        return false;
    }
}
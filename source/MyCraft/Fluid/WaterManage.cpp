#include "WaterManage.h"
#include "ChunkManage.h"

namespace MyCraft {
    
    WaterManage::WaterManage(ChunkManage& WorldRender): __manage(WorldRender) {
        __flowClock.setDuration(1000);
    }
    WaterManage::~WaterManage() {}
    void WaterManage::place(const glm::ivec3& position) {
        if (__manage.getWaterHeight(position)<1) {
            __manage.setWater(position, glm::vec4(1));
            __waterFlow.push(position);
        }
    }
    bool WaterManage::handle(GLFWwindow* window) {
        if (__flowClock.get() && __waterFlow.size()) {
            __flowClock.restart();
            int size = __waterFlow.size();
            for (int i = 0; i<size; i++) {
                glm::ivec3 position = __waterFlow.front();
                __waterFlow.pop();
                float height = __manage.getWaterHeight(position);
                if (height) {
                    glm::ivec3 cur = position+glm::ivec3(1,0,0);
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, glm::vec4(height-0.2));
                        __waterFlow.push(cur);
                    }
                    cur.x -= 2;
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, glm::vec4(height-0.2));
                        __waterFlow.push(cur);
                    }

                    cur += glm::ivec3(1, 1, 0);
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, glm::vec4(height-0.2));
                        __waterFlow.push(cur);
                    }

                    cur.y -= 2;
                    if (__manage.getWaterHeight(cur)<height-0.2) {
                        __manage.setWater(cur, glm::vec4(height-0.2));
                        __waterFlow.push(cur);
                    }
                }
                else {
                    glm::ivec3 cur = position+glm::ivec3(1,0,0);
                    if (!__manage.getWaterHeight(cur)) {
                        __manage.setWater(cur, glm::vec4(0));
                    }
                    cur.x -= 2;
                    if (!__manage.getWaterHeight(cur)) {
                        __manage.setWater(cur, glm::vec4(0));
                    }

                    cur += glm::ivec3(1, 1, 0);
                    if (!__manage.getWaterHeight(cur)) {
                        __manage.setWater(cur, glm::vec4(0));
                    }

                    cur.y -= 2;
                    if (!__manage.getWaterHeight(cur)) {
                        __manage.setWater(cur, glm::vec4(0));
                    }
                }
            }
        }
        return false;
    }
}
#include "World.h"
#include "Block.h"
#include "GLFW/glfw3.h"
namespace MyCraft {
    World::World(const int& x, const int& y, const int& z): pPosition(x-world_side*16, y-world_side*16, z-world_side*16) {
        for (int i = 0; i<world_side*2+1; i++) {
            for (int j = 0; j<world_side*2+1; j++) {
                for (int k = 0; k<world_side*2+1; k++) {
                    pChunks[i][j][k].setPosition(pPosition.x+i*16, pPosition.y+j*16, pPosition.z+k*16);
                }
            }
        }
        hX = hY = hZ = 0;
        pFrameAlarm.setDuration(150);
    }
    World::~World() {
    }
    bool World::setHover(const MyBase3D::Ray3f& ray) {
        bool hover = false;
        glm::vec3 pos = ray.getOrigin();
        glm::vec3 dir = ray;
        if (isHovered()) {
            at(hX, hY, hZ).setHoverPlane(-1);
        }
        glm::vec3 limit = abs(dir);
        float pPlane = -1, pMinDistance = glm::length(dir);
        if (dir.x>0) {
            for (int i = 0; i<limit.x; i++) {
                float planeX = ceil(pos.x)+i;
                glm::vec3 delta = ray.distance(0, planeX);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (Block& tmp = at(planeX, floor(center.y), floor(center.z)); tmp.getType() != BlockCatogary::Air) {
                        hX = planeX;
                        hY = floor(center.y);
                        hZ = floor(center.z);
                        pMinDistance = glm::length(delta);
                        pPlane = 0;
                        hover = true;
                        break;
                    }
                }
            }
        }
        else if (dir.x<0) {
            for (int i = 0; i<limit.x; i++) {
                float planeX = floor(pos.x)-i;
                glm::vec3 delta = ray.distance(0, planeX);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (at(planeX-1, floor(center.y), floor(center.z)).getType() != BlockCatogary::Air) {
                        hX = planeX-1;
                        hY = floor(center.y);
                        hZ = floor(center.z);
                        pPlane=1;
                        pMinDistance = glm::length(delta);
                        hover = true;
                        break;
                    }
                }
            }
        }
        if (dir.y>0) {
            for (int i = 0; i<limit.y; i++) {
                float planeY = ceil(pos.y)+i;
                glm::vec3 delta = ray.distance(1, planeY);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (at(floor(center.x), planeY, floor(center.z)).getType() != BlockCatogary::Air) {
                        hX = floor(center.x);
                        hY = planeY;
                        hZ = floor(center.z);
                        pPlane=2;
                        pMinDistance = glm::length(delta);
                        hover = true;
                        break;
                    }
                }
            }
        }
        else if (dir.y<0) {
            for (int i = 0; i<limit.y; i++) {
                float planeY = floor(pos.y)-i;
                glm::vec3 delta = ray.distance(1, planeY);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (at(floor(center.x), planeY-1, floor(center.z)).getType() != BlockCatogary::Air) {
                        hX = floor(center.x);
                        hY = planeY-1;
                        hZ = floor(center.z);
                        pPlane=3;
                        pMinDistance = glm::length(delta);
                        hover = true;
                        break;
                    }
                }
            }
        }
        if (dir.z>0) {
            for (int i = 0; i<limit.z; i++) {
                float planeZ = ceil(pos.z)+i;
                glm::vec3 delta = ray.distance(2, planeZ);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (at(floor(center.x), floor(center.y), planeZ).getType() != BlockCatogary::Air) {
                        hX = floor(center.x);
                        hY = floor(center.y);
                        hZ = planeZ;
                        pPlane = 4;
                        pMinDistance = glm::length(delta);
                        hover = true;
                        break;
                    }
                }
            }
        }
        else if (dir.z<0) {
            for (int i = 0; i<limit.z; i++) {
                float planeZ = floor(pos.z)-i;
                glm::vec3 delta = ray.distance(2, planeZ);
                if (glm::length(delta)<=pMinDistance) {
                    glm::vec3 center = delta+pos;
                    if (at(floor(center.x), floor(center.y), planeZ-1).getType() != BlockCatogary::Air) {
                        hX = floor(center.x);
                        hY = floor(center.y);
                        hZ = planeZ-1;
                        pPlane = 5;
                        pMinDistance = glm::length(delta);
                        hover = true;
                        break;
                    }
                }
            }
        }
        if (hover) {
            at(hX, hY, hZ).setHoverPlane(pPlane);
        }
        Controller3D::setHover(hover);
        return true;
    }
    
    bool World::handle(GLFWwindow* window) {
        bool is_changed = Controller3D::handle(window);
        if (pFrameAlarm.get()) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                if (isHovered()) {
                    Block& pCurrentBlock = at(hX, hY, hZ);
                    float nX = hX, nY = hY, nZ = hZ;
                    switch (pCurrentBlock.getHoverPlane()) {
                        case 0: nX--; break;
                        case 1: nX++; break;
                        case 2: nY--; break;
                        case 3: nY++; break;
                        case 4: nZ--; break;
                        case 5: nZ++; break;
                    }
                    at(nX, nY, nZ).setType(BlockCatogary::Dirt);
                    is_changed = true;
                }
            }
        }
        return is_changed;
    }
    Block& World::at(const int& x, const int& y, const int& z) {
        int rX = (x-pPosition.x);
        int rY = (y-pPosition.y);
        int rZ = (z-pPosition.z);
        return pChunks[rX/16][rY/16][rZ/16].at(rX%16, rY%16, rZ%16);
    }
    void World::glDraw() const {
        for (int i = 0; i<world_side*2+1; i++) {
            for (int j = 0; j<world_side*2+1; j++) {
                for (int k = 0; k<world_side*2+1; k++) {
                    pChunks[i][j][k].glDraw();
                }
            }
        }
    }
}
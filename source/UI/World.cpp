#include "World.h"
#include "Block.h"
#include "Chunk.h"
#include "Message.h"
#include "GLFW/glfw3.h"
#include "General.h"
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
        add(new CheckFall(this));
        add(new CheckEmpty(this));
    }
    World::~World() {
    }
    // bool World::setHover(const MyBase3D::Ray3f& ray) {
    //     bool hover = false;
    //     glm::vec3 pos = ray.getOrigin();
    //     glm::vec3 dir = ray;
    //     if (isHovered()) {
    //         at(hX, hY, hZ).setHoverPlane(-1);
    //     }
    //     glm::vec3 limit = abs(dir);
    //     float pPlane = -1, pMinDistance = glm::length(dir);
    //     if (dir.x>0) {
    //         for (int i = 0; i<limit.x; i++) {
    //             float planeX = ceil(pos.x)+i;
    //             glm::vec3 delta = ray.distance(0, planeX);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (Block& tmp = at(planeX, floor(center.y), floor(center.z)); tmp.getType() != BlockCatogary::Air) {
    //                     hX = planeX;
    //                     hY = floor(center.y);
    //                     hZ = floor(center.z);
    //                     pMinDistance = glm::length(delta);
    //                     pPlane = 0;
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     else if (dir.x<0) {
    //         for (int i = 0; i<limit.x; i++) {
    //             float planeX = floor(pos.x)-i;
    //             glm::vec3 delta = ray.distance(0, planeX);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (at(planeX-1, floor(center.y), floor(center.z)).getType() != BlockCatogary::Air) {
    //                     hX = planeX-1;
    //                     hY = floor(center.y);
    //                     hZ = floor(center.z);
    //                     pPlane=1;
    //                     pMinDistance = glm::length(delta);
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     if (dir.y>0) {
    //         for (int i = 0; i<limit.y; i++) {
    //             float planeY = ceil(pos.y)+i;
    //             glm::vec3 delta = ray.distance(1, planeY);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (at(floor(center.x), planeY, floor(center.z)).getType() != BlockCatogary::Air) {
    //                     hX = floor(center.x);
    //                     hY = planeY;
    //                     hZ = floor(center.z);
    //                     pPlane=2;
    //                     pMinDistance = glm::length(delta);
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     else if (dir.y<0) {
    //         for (int i = 0; i<limit.y; i++) {
    //             float planeY = floor(pos.y)-i;
    //             glm::vec3 delta = ray.distance(1, planeY);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (at(floor(center.x), planeY-1, floor(center.z)).getType() != BlockCatogary::Air) {
    //                     hX = floor(center.x);
    //                     hY = planeY-1;
    //                     hZ = floor(center.z);
    //                     pPlane=3;
    //                     pMinDistance = glm::length(delta);
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     if (dir.z>0) {
    //         for (int i = 0; i<limit.z; i++) {
    //             float planeZ = ceil(pos.z)+i;
    //             glm::vec3 delta = ray.distance(2, planeZ);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (at(floor(center.x), floor(center.y), planeZ).getType() != BlockCatogary::Air) {
    //                     hX = floor(center.x);
    //                     hY = floor(center.y);
    //                     hZ = planeZ;
    //                     pPlane = 4;
    //                     pMinDistance = glm::length(delta);
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     else if (dir.z<0) {
    //         for (int i = 0; i<limit.z; i++) {
    //             float planeZ = floor(pos.z)-i;
    //             glm::vec3 delta = ray.distance(2, planeZ);
    //             if (glm::length(delta)<=pMinDistance) {
    //                 glm::vec3 center = delta+pos;
    //                 if (at(floor(center.x), floor(center.y), planeZ-1).getType() != BlockCatogary::Air) {
    //                     hX = floor(center.x);
    //                     hY = floor(center.y);
    //                     hZ = planeZ-1;
    //                     pPlane = 5;
    //                     pMinDistance = glm::length(delta);
    //                     hover = true;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    //     if (hover) {
    //         at(hX, hY, hZ).setHoverPlane(pPlane);
    //     }
    //     Controller3D::setHover(hover);
    //     return true;
    // }
    
    bool World::handle(GLFWwindow* window) {
        bool is_changed = Controller::handle(window);
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
    Block& World::at(const glm::vec3& pos) {
        return at(std::floor(pos.x), std::floor(pos.y), std::floor(pos.z));
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

    CheckEmpty::CheckEmpty(World* world): __world(world) {}
    CheckEmpty::~CheckEmpty() {}
    
    void CheckEmpty::execute(Port& mine, Port& source, Message* message) {
        RequestGoto* request = (RequestGoto*)message;
        bool below_result = true, above_result = true;
        auto& shape = request->rectangleBox;
        glm::vec3 dir = glm::vec3(request->direction, 0);
        //Below check
        glm::vec3 npos = shape[0] + dir, epos = npos + shape[1];
        std::queue<glm::vec3> q = rasterize(npos, epos);
        while (q.size() && below_result) {
            if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
            q.pop();
        }
        //Above block check
        npos.z += 1;
        epos.z += 1;
        q = rasterize(npos, epos);
        while (q.size() && above_result) {
            if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
            q.pop();
        }
        if (!below_result || !above_result) {
            //Check auto jump
            if (above_result) {
                below_result = above_result = true;
                q = rasterize(shape[0]-glm::vec3(0,0,1), shape[0]+shape[1]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world->at(q.front()).getType() == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                q = rasterize(shape[0]+shape[2]-glm::vec3(0,0,1), shape[0]+shape[1]+shape[2]-glm::vec3(0,0,1));
                while (q.size() && above_result) {
                    if (__world->at(q.front()).getType() == BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z += 1;
                epos.z += 1;
                q = rasterize(npos, epos);
                while (q.size() && above_result) {
                    if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
                    q.pop();
                }
            }
            if (below_result && above_result) dir.z=1;
            else {
                //Check parallel Ox
                npos = shape[0] + glm::vec3(dir.x, 0, 0); epos = npos + shape[1];
                below_result = above_result = true;
                q = rasterize(npos, epos);
                while (q.size() && below_result) {
                    if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                    q.pop();
                }
                npos.z++; epos.z++;
                q = rasterize(npos, epos);
                while (q.size() && below_result) {
                    if (__world->at(q.front()).getType() != BlockCatogary::Air) above_result = false;
                    q.pop();
                }

                if (above_result && below_result) dir.y = 0;
                else {
                    //Check paralel Oy
                    npos = shape[0] + glm::vec3(0, dir.y, 0); epos = npos + shape[1];
                    below_result = above_result = true;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    npos.z++; epos.z++;
                    q = rasterize(npos, epos);
                    while (q.size() && below_result) {
                        if (__world->at(q.front()).getType() != BlockCatogary::Air) below_result = false;
                        q.pop();
                    }
                    if (below_result) dir.x = 0;
                }
            }
        }
        if (below_result) {
            mine.send(source, new Move(dir));

            shape[3] = shape[0] + shape[2];
            shape[3][2]--;
            shape[2] += shape[0] + shape[1];
            shape[2][2]--;
            shape[1] += shape[0];
            shape[1][2]--;
            shape[0][2]--;
        }
    }
    MessageType CheckEmpty::getType() const {
        return MessageType::RequestGotoMessage;
    }
    
    CheckFall::CheckFall(World* world): __world(world) {}
    CheckFall::~CheckFall() {}
        
    void CheckFall::execute(Port& mine, Port& source, Message* message) {
        RequestFall* request = (RequestFall*)message;
        float z = request->zVelocity;
        auto& shape = request->rectangleBox;
        if (z<=0) {
            z -= 0.06;
            bool isFall = true;
            shape[3] = shape[0] + shape[2];
            isFall = isFall && (__world->at(shape[3]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                __world->at(shape[3]).getType()!=BlockCatogary::Air); 
            shape[2] += shape[0] + shape[1];
            isFall = isFall && (__world->at(shape[2]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                __world->at(shape[2]).getType()!=BlockCatogary::Air); 
            shape[1] += shape[0];
            isFall = isFall && (__world->at(shape[1]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                __world->at(shape[1]).getType()!=BlockCatogary::Air); 
            isFall = isFall && (__world->at(shape[0]+glm::vec3(0,0,z)).getType()==BlockCatogary::Air || 
                                __world->at(shape[0]).getType()!=BlockCatogary::Air); 
            if (isFall) mine.send(source, new Fall(z));
            else {
                float delta = shape[0][2] - floor(shape[0][2]);
                mine.send(source, new Fall(-delta));
                mine.send(source, new StopFall());
            }
        }
        else if (z>0) {
            shape[0] += shape[3];

            shape[3] = shape[0] + shape[2];
            shape[3][2]++;
            shape[2] += shape[0] + shape[1];
            shape[2][2]++;
            shape[1] += shape[0];
            shape[1][2]++;
            shape[0][2]++;
            if (__world->at(shape[0]).getType()==BlockCatogary::Air && 
                __world->at(shape[1]).getType()==BlockCatogary::Air && 
                __world->at(shape[2]).getType()==BlockCatogary::Air && 
                __world->at(shape[3]).getType()==BlockCatogary::Air) {
                    mine.send(source, new Fall(z-0.035));
            }
            else {
                float delta = floor(shape[0][2]) - shape[0][2] + 1;
                if (delta>=0.01) delta -= 0.01;
                mine.send(source, new Fall(delta));
            }
        }
    }
    MessageType CheckFall::getType() const {
        return MessageType::RequestFallMessage;
    }
}
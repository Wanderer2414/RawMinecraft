#include "GameForm.h"
#include "Block.h"
#include "HitBoxCenter.h"
#include "InfoCenter.h"
#include <GL/gl.h>

namespace MyCraft {
    GameForm::GameForm(GLFWwindow* window, const int& index): Form3D(index), pWorld(0, 0, 0) {
        insert(&pWorld);
        insert(&__model);
        insert(&__hitbox);
        __hitbox.insert(&__model);
        __hitbox.setWorld(&pWorld);
        for (int i = -16; i<16; i++) {
            for (int j = -16; j<16; j++) {
                pWorld.at(i, j, -1).setType(BlockCatogary::Grass);
            }
        }
        for (int i = -16; i<0; i++) {
            for (int j = -16; j<0; j++) pWorld.at(i,j,0).setType(BlockCatogary::Grass);
        }
        pWorld.at(5,5,0).setType(BlockCatogary::Grass);
        pWorld.at(5,5,1).setType(BlockCatogary::Grass);
        pWorld.at(5,6,0).setType(BlockCatogary::Grass);
        pWorld.at(5,6,1).setType(BlockCatogary::Grass);
        pWorld.at(5,7,0).setType(BlockCatogary::Grass);
        pWorld.at(5,7,1).setType(BlockCatogary::Grass);

        pWorld.at(3,5,0).setType(BlockCatogary::Grass);
        pWorld.at(3,5,1).setType(BlockCatogary::Grass);
        pWorld.at(3,6,0).setType(BlockCatogary::Grass);
        pWorld.at(3,6,1).setType(BlockCatogary::Grass);
        pWorld.at(3,7,0).setType(BlockCatogary::Grass);
        pWorld.at(3,7,1).setType(BlockCatogary::Grass);

        pWorld.at(5,3,0).setType(BlockCatogary::Grass);
        pWorld.at(5,3,1).setType(BlockCatogary::Grass);
        pWorld.at(6,3,0).setType(BlockCatogary::Grass);
        pWorld.at(6,3,1).setType(BlockCatogary::Grass);
        pWorld.at(7,3,0).setType(BlockCatogary::Grass);
        pWorld.at(7,3,1).setType(BlockCatogary::Grass);

        pWorld.at(5,5,0).setType(BlockCatogary::Grass);
        pWorld.at(5,5,1).setType(BlockCatogary::Grass);
        pWorld.at(6,5,0).setType(BlockCatogary::Grass);
        pWorld.at(6,5,1).setType(BlockCatogary::Grass);
        pWorld.at(7,5,0).setType(BlockCatogary::Grass);
        pWorld.at(7,5,1).setType(BlockCatogary::Grass);

        pWorld.at(-5,-5,0).setType(BlockCatogary::Grass);
        pWorld.at(-5,-5,1).setType(BlockCatogary::Grass);
    
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window, InfoCenter::Default->getWindowHalf().x, InfoCenter::Default->getWindowHalf().y);
    
        _camera.setPosition({10, 10, 1.7});
        pZVelocity = 0;
        pSpeed = 0.1;
        pFrameAlarm.setDuration(50);
    }
    GameForm::~GameForm() {        
    }
    bool GameForm::move(const float& x, const float& y, const float& z) {
        glm::vec3 delta = {0, 0, 0}, pos= _camera.getPosition();
        delta += x*_camera.getHorizontalVector();
        glm::vec3 tmp = _camera.getDirection();
        tmp.z = 0;
        tmp /= glm::length(tmp);
        delta.x += y*tmp.x;
        delta.y += y*tmp.y;
        delta.z += z;
        // if (!z) {
        //     float cX = pos.x+delta.x;
        //     int fX = floor(pos.x), fY = floor(pos.y);
        //     float cY = pos.y+delta.y;
        //     int fZ = floor(pos.z-0.7);
        //     if (delta.x>0) cX += 0.3;
        //     if (delta.x<0) cX -= 0.3;
        //     if (delta.y>0) cY += 0.3;
        //     if (delta.y<0) cY -= 0.3;
        //     cX = floor(cX);
        //     cY = floor(cY);
        //     if (cX != fX || cY!=fY) {
        //         bool above_x = pWorld.at(cX, fY, fZ).getType() == BlockCatogary::Air;
        //         bool above_y = pWorld.at(fX, cY, fZ).getType() == BlockCatogary::Air;
        //         bool below_x = pWorld.at(cX, fY, fZ-1).getType() == BlockCatogary::Air;
        //         bool below_y = pWorld.at(fX, cY, fZ - 1).getType() == BlockCatogary::Air;
        //         bool above_xy = pWorld.at(cX, cY,fZ).getType() == BlockCatogary::Air;
        //         bool below_xy = pWorld.at(cX, cY, fZ-1).getType() == BlockCatogary::Air;
        //         if (!above_x || !below_x) {
        //             if (delta.x<0) delta.x = fX+0.3-pos.x;
        //             else if (delta.x>0) delta.x = cX-0.3-pos.x;
        //         }
        //         if (!above_y || !below_y) {
        //             if (delta.y<0) delta.y = fY+0.3-pos.y;
        //             else if (delta.y>0) delta.y = cY-0.3-pos.y;
        //         }
        //         if (above_x && above_y && !above_xy) {
        //             if (delta.x<0) delta.x = fX+0.3-pos.x;
        //             else if (delta.x>0) delta.x = cX-0.3-pos.x;
        //             if (delta.y<0) delta.y = fY+0.3-pos.y;
        //             else if (delta.y>0) delta.y = cY-0.3-pos.y;
        //         }
        //         if ((above_x && !below_x) || (above_y && !below_y) || (above_xy && !below_xy)) {
        //             if (    pWorld.at(cX, cY, fZ-2).getType()!=BlockCatogary::Air && 
        //                     pWorld.at(fX, fY, fZ-2).getType()!=BlockCatogary::Air &&
        //                     pWorld.at(cX, cY, fZ+1).getType() == BlockCatogary::Air) {
        //                 if (delta.x>0) delta.x+=0.1;
        //                 else if (delta.x<0) delta.x-=0.1;
        //                 if (delta.y>0) delta.y+=0.1;
        //                 else if (delta.y<0) delta.y-=0.1;
        //                 delta.z = 1;
        //             }
        //         }
        //     }
        //     _camera.setPosition(delta+pos);
        //     return true;
        // }
        // else {
        //     if (pWorld.at(floor(pos.x), floor(pos.y), floor(pos.z+delta.z-1.7)).getType() == BlockCatogary::Air) {
        //         _camera.setPosition(delta+pos);
        //         return true;
        //     }
        //     return false;
        // };
        // __model.move(delta);
        return true;
    }
    bool GameForm::handle(GLFWwindow* window) {
        bool is_changed = Form3D::handle(window);
    
        if (pFrameAlarm.get()) {
            pFrameAlarm.restart();
            // if (pZVelocity>-40) {
            //     if (glfwGetKey(
            // if (pZVelocity>-50 &&   pWorld.at(floor(_camera.getPosition().x-0.25), floor(_camera.getPosition().y-0.25), floor(_camera.getPosition().z-2)).getType() == BlockCatogary::Air &&
            //                         pWorld.at(floor(_camera.getPosition().x-0.25), floor(_camera.getPosition().y+0.25), floor(_camera.getPosition().z-2)).getType() == BlockCatogary::Air &&
            //                         pWorld.at(floor(_camera.getPosition().x+0.25), floor(_camera.getPosition().y+0.25), floor(_camera.getPosition().z-2)).getType() == BlockCatogary::Air &&
            //                         pWorld.at(floor(_camera.getPosition().x+0.25), floor(_camera.getPosition().y-0.25), floor(_camera.getPosition().z-2)).getType() == BlockCatogary::Air) {
            //     pZVelocity -= 3;
            // }
            // else if (!pZVelocity && glfwGetKey(window, GLFW_KEY_SPACE)) {
            //     pZVelocity = 30;
            // }
            // if (pZVelocity>0) {
            //     move(0, 0, pZVelocity/150);
            //     is_changed = true;
            // }
            // else if (pZVelocity<0) {
            //     if (!move(0, 0, pZVelocity/200)) {
            //         pZVelocity = 0;
            //     };
            //     is_changed = true;
            // }
        }
        glm::vec3 dir = __model.getDirection();
        dir.z -= 2;
        _camera.setPosition(__model.getPosition() - 3.f*dir);
        _camera.look(__model.getPosition()+glm::vec3(0, 0, 3));
    
        glm::vec<2, double> position;
        glfwGetCursorPos(window, &position.x, &position.y);
        glm::vec2 delta = position;
        delta -= InfoCenter::Default->getWindowHalf();
    
        if (delta.x != 0 || delta.y != 0) {
            glfwSetCursorPos(window, InfoCenter::Default->getWindowHalf().x, InfoCenter::Default->getWindowHalf().y);
            __model.seeRotate(-delta.x/1000);
            is_changed = true;
        }
        return true;
    }
}
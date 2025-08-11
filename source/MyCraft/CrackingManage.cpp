#include "CrackingManage.h"
#include "Block.h"
#include "DrawingCenter.h"
#include <cmath>

namespace MyCraft {
    CrackingManage::CrackingManage(): __crackingBlock(0), __isCracking(false) {
        __crackingTexture.load("assets/images/cracking.png", false);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        __clock.setDuration(300);
        __gravityClock.setDuration(3);
    }
    CrackingManage::~CrackingManage() {}

    float CrackingManage::getPercent() const {
        return __percent;
    }
    BlockCatogary CrackingManage::getType() const {
        return __type;
    }
    void CrackingManage::crack(const float& percent) {
        __clock.restart();
        __percent += percent;
        __crackingBlocks.setType(0, BlockCatogary(round(__percent*3)));
        for (int i = 0; i<ceil(__percent*32) - __crackingPieces.size(); i++) {
            __crackingPieces.push(glm::mat4({0.1,0,0,0}, 
                                            {0, 0.1, 0, 0}, 
                                            {0, 0, 0.1, 0}, 
                {rand()%110/100.f - 0.05 + __crackingBlock.x, rand()%110/100.f - 0.05 + __crackingBlock.y,rand()%110/100.f - 0.05 + __crackingBlock.z, 1}), 
                                                {0,0,0,__type});
        }
        if (__percent>=1) {
            __percent = 0;
            for (int i = 0; i<32; i++) {
            __crackingPieces.push(
                                  glm::mat4({0.1,0,0,0}, 
                                            {0, 0.1, 0, 0}, 
                                            {0, 0, 0.1, 0}, 
                {rand()%110/100.f - 0.05 + __crackingBlock.x, rand()%110/100.f - 0.05 + __crackingBlock.y,__crackingBlock.z + i/32.f, 1}), 
                                                {0,0,0,__type});
            }
        }
    }
    void CrackingManage::setCrackBlock(const glm::ivec3& hover, const BlockCatogary& type) {
        __isCracking = true;
        if (__crackingBlock != hover) {
            __percent = 0;
            __type = type;
            __crackingBlock= hover;
            __crackingBlocks.clear();
            __crackingBlocks.push(glm::mat4({1.02, 0,0,0}, {0, 1.02, 0, 0}, {0,0,1.02, 0}, {glm::vec3(__crackingBlock)-glm::vec3(0.01), 1}), {0,0,0, type});
        }
    }
    void CrackingManage::uncrack() {
        __isCracking = false;
        __crackingBlocks.clear();
    }
    bool CrackingManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        if (__clock.get() && __isCracking) {
            __percent = 0;
            __isCracking = false;
            is_changed = true;
        }
        if (__gravityClock.get() && __crackingPieces.size()) {
            __gravityClock.restart();
            for (int i = 0; i<__crackingPieces.size(); i++) {
                __crackingPieces.getState(i)[3].z-=0.01;
                if (__crackingBlock.z - __crackingPieces.getState(i)[3].z > 5) {
                    __crackingPieces.remove(i);
                }
            }
            is_changed = true;
        }

        
        return is_changed;
    }
    glm::ivec3 CrackingManage::getCrackingBlock() const {
        return __crackingBlock;
    }
    void CrackingManage::glDrawTransparent() const {

        if (__percent>0 && __isCracking) {
            glm::vec4 info(0,0,0,round(__percent*3));
            DrawingCenter::BindCube(__crackingTexture, {1,1.02});
            DrawingCenter::DrawCubes(__crackingBlocks);

        }
        if (__crackingPieces.size()) {
            DrawingCenter::BindCube(__blockTexture, glm::vec2(0.3, 0.1));
            DrawingCenter::DrawCubes(__crackingPieces);
        }
    }
}
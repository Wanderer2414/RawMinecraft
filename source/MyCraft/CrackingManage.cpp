#include "CrackingManage.h"
#include "Block.h"
#include "Controller.h"
#include "DrawingCenter.h"
#include "Inventory.h"
#include <cmath>

namespace MyCraft {
    CrackingManage::CrackingManage(): __crackingBlock(0), __numberRemain(0), __numberPieces(0), __isCracking(false) {
        __crackingTexture.load("assets/images/cracking.png", false);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        __clock.setDuration(300);
        __gravityClock.setDuration(3);
        __crackingPieces = new glm::mat4[64];
        for (int i = 0; i<64; i++) {
            __crackingPieces[i][0] = {0.1, 0,0,0};
            __crackingPieces[i][1] = {0, 0.1,0,0};
            __crackingPieces[i][2] = {0, 0,0.1,0};
        }
    }
    CrackingManage::~CrackingManage() {
        delete[] __crackingPieces;
    }

    float CrackingManage::getPercent() const {
        return __percent;
    }
    BlockCatogary CrackingManage::getType() const {
        return __type;
    }
    void CrackingManage::crack(const float& percent) {
        __clock.restart();
        if (!__percent) {
            for (int i = 0; i<32; i++) {
                __crackingPieces[i][3] = {rand()%110/100.f - 0.05 + __crackingBlock.x, rand()%110/100.f - 0.05 + __crackingBlock.y, rand()%110/100.f - 0.05 + __crackingBlock.z, __type};
            }
        }
        __percent += percent;
        if (__percent<1) {
            __numberPieces = ceil(32*__percent);
        }
        else if (__numberRemain<64) {
            __percent = 0;
            __numberRemain = 32;
            for (int i = 0; i<32; i++) {
                __crackingPieces[i+32][3] = {rand()%110/100.f - 0.05 + __crackingBlock.x, rand()%110/100.f - 0.05 + __crackingBlock.y, __crackingBlock.z + i*0.1/3, __type};
            }
        }
    }
    void CrackingManage::setCrackBlock(const glm::ivec3& hover, const BlockCatogary& type) {
        __isCracking = true;
        if (__crackingBlock != hover) {
            __percent = 0;
            __type = type;
            __crackingBlock= hover;
        }
    }
    void CrackingManage::uncrack() {
        __isCracking = false;
    }
    bool CrackingManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        if (__clock.get()) {
            __percent = 0;
            __isCracking = false;
            is_changed = true;
        }
        if (__gravityClock.get() && (__numberPieces || __numberRemain)) {
            __gravityClock.restart();
            if (__numberPieces) {
                for (int i = __numberPieces-1; i>=0; i--) {
                    __crackingPieces[i][3].z-=0.01;
                    if (__crackingBlock.z - __crackingPieces[i][3].z > 3) {
                        std::swap(__crackingPieces[i], __crackingPieces[__numberPieces-1]);
                        __numberPieces--;
                    }
                }
                is_changed = true;
            }

            if (__numberRemain) {
                for (int i = 32+__numberRemain-1; i>=32; i--) {
                    __crackingPieces[i][3].z-=0.01;
                    if (__crackingBlock.z - __crackingPieces[i][3].z > 5) {
                        std::swap(__crackingPieces[i], __crackingPieces[32+__numberRemain-1]);
                        __numberRemain--;
                    }
                }
                is_changed = true;
            }

        }
        return is_changed;
    }
    glm::ivec3 CrackingManage::getCrackingBlock() const {
        return __crackingBlock;
    }
    void CrackingManage::glDrawTransparent() const {

        if (__percent>0 && __isCracking) {
            glm::mat4 margin(1);
            margin[3] = {__crackingBlock.x-0.01, __crackingBlock.y-0.01, __crackingBlock.z-0.01, round(__percent*3)};
            margin[0] = {1.02, 0, 0, 0};
            margin[1] = {0, 1.02, 0, 0};
            margin[2] = {0, 0, 1.02,0};
            DrawingCenter::BindCube(__crackingTexture, {1,1.02});
            DrawingCenter::DrawCubes(&margin, 1);

        }
        if (__numberPieces || __numberRemain) {
            DrawingCenter::BindCube(__blockTexture, glm::vec2(0.3, 0.1));
            if (__numberPieces) {
                DrawingCenter::DrawCubes(__crackingPieces, __numberPieces);
            }
            if (__numberRemain) {
                DrawingCenter::BindCube(__blockTexture, glm::vec2(0.3, 0.1));
                DrawingCenter::DrawCubes(__crackingPieces+32, __numberRemain);
            }
        }
    }
}
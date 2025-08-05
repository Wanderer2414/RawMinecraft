#include "CrackingManage.h"
#include "Block.h"
#include "Controller.h"
#include "DrawingCenter.h"
#include <cmath>

namespace MyCraft {
    CrackingManage::CrackingManage(): __hoverBlock(0), __isHover(false), __numberRemain(0), __numberPieces(0) {
        setVisible(false);
        __crackingTexture.load("assets/images/cracking.png", false);
        __blockTexture.load("assets/images/blockCatogary.png", false);
        __clock.setDuration(250);
        __gravityClock.setDuration(3);
        __crackingPieces = new glm::vec4[64];
    }
    CrackingManage::~CrackingManage() {
        delete[] __crackingPieces;
    }

    bool CrackingManage::isHover() const {
        return __isHover;
    }
    float CrackingManage::getPercent() const {
        return __percent;
    }
    glm::ivec3 CrackingManage::getHoverBlock() const {
        return __hoverBlock;
    }
    void CrackingManage::crack(const float& percent) {
        __clock.restart();
        if (__percent==0) {
            for (int i = 0; i<32; i++) {
                __crackingPieces[i] = {rand()%110/100.f - 0.05 + __hoverBlock.x, rand()%110/100.f - 0.05 + __hoverBlock.y, rand()%110/100.f - 0.05 + __hoverBlock.z, __type};
            }
        }
        __percent += percent;
        if (__percent<0.9) {
            __numberPieces = ceil(32*__percent);
        }
        else if (!__numberRemain) {
            __numberRemain = 32;
            for (int i = 0; i<32; i++) {
                __crackingPieces[i+32] = {rand()%110/100.f - 0.05 + __hoverBlock.x, rand()%110/100.f - 0.05 + __hoverBlock.y, __hoverBlock.z + i*0.1/3, __type};
            }
        }
    }
    void CrackingManage::hover() {
        __isHover = true;
        setVisible(true);
    } 
    void CrackingManage::unhover() {
        __percent = 0;
        __isHover = false;
        if (!__numberRemain && !__numberPieces) setVisible(false);
    }
    void CrackingManage::setHoverBlock(const glm::ivec3& hover, const BlockCatogary& type) {
        __type = type;
        __hoverBlock= hover;
    }
    bool CrackingManage::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Controller3D::handle(window);
        if (__clock.get()) {
            __percent = 0;
            is_changed = true;
        }
        if (__gravityClock.get() && (__numberPieces || __numberRemain)) {
            __gravityClock.restart();
            if (__numberPieces) {
                for (int i = __numberPieces-1; i>=0; i--) {
                    __crackingPieces[i].z-=0.01;
                    if (__hoverBlock.z - __crackingPieces[i].z > 3) {
                        std::swap(__crackingPieces[i], __crackingPieces[__numberPieces-1]);
                        __numberPieces--;
                    }
                }
                is_changed = true;
            }

            if (__numberRemain) {
                for (int i = 32+__numberRemain-1; i>=32; i--) {
                    __crackingPieces[i].z-=0.01;
                    if (__hoverBlock.z - __crackingPieces[i].z > 5) {
                        std::swap(__crackingPieces[i], __crackingPieces[32+__numberRemain-1]);
                        __numberRemain--;
                    }
                }
                if (!__numberRemain) setVisible(false);
                is_changed = true;
            }

        }
        return is_changed;
    }

    void CrackingManage::glDrawTransparent() const {
        if (__isHover) {
            glm::vec4 margin( __hoverBlock, 1);
            DrawingCenter::BindMargin();
            DrawingCenter::DrawMargins(&margin, 1, BLACK, 3);
        }

        if (__percent>0) {
            glm::vec4 margin( __hoverBlock, 1);
            margin.x -= 0.01;
            margin.y -= 0.01;
            margin.z -= 0.01;
            margin.w = round(__percent*4);
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
#ifndef CRACKING_MANAGE_H
#define CRACKING_MANAGE_H
#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "Global.h"
#include "Texture.h"
namespace MyCraft {
    class CrackingManage: public MyBase3D::Controller3D {
    public:
        CrackingManage();
        ~CrackingManage();
        CrackingManage(const CrackingManage&) = delete;
        CrackingManage& operator=(const CrackingManage&) const = delete;

        bool isHover() const;
        float getPercent() const;
        void crack(const float& percent);
        glm::ivec3 getHoverBlock() const;
        void hover(), unhover();
        void setHoverBlock(const glm::ivec3& hover,const BlockCatogary& type);
    protected:
    private:
        bool                    __isHover;
        float                   __percent;
        int                     __numberPieces, __numberRemain;
        BlockCatogary           __type;
        glm::ivec3              __hoverBlock;
        MyBase::Texture         __crackingTexture, __blockTexture;
        MyBase::Clock           __clock, __gravityClock;
        glm::vec4             *__crackingPieces;

        bool handle(GLFWwindow*)       override;
        void glDrawTransparent() const override;
    };
}
#endif
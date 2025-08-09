#ifndef CRACKING_MANAGE_H
#define CRACKING_MANAGE_H
#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "Global.h"
#include "Item.h"
#include "Message.h"
#include "Texture.h"
namespace MyCraft {
    class CrackingManage: public MyBase3D::Controller3D, public MyBase::Port {
    public:
        CrackingManage();
        ~CrackingManage();
        CrackingManage(const CrackingManage&) = delete;
        CrackingManage& operator=(const CrackingManage&) const = delete;
        BlockCatogary getType() const;
        glm::ivec3 getCrackingBlock() const;
        float getPercent() const;
        void crack(const float& percent);
        void uncrack();
        void setCrackBlock(const glm::ivec3& hover,const BlockCatogary& type);
    protected:
    private:
        bool                    __isCracking;
        float                   __percent;
        int                     __numberPieces, __numberRemain;
        BlockCatogary           __type;
        glm::ivec3              __crackingBlock;
        MyBase::Texture         __crackingTexture, __blockTexture;
        MyBase::Clock           __clock, __gravityClock;
        glm::mat4               *__crackingPieces;

        bool handle(GLFWwindow*)       override;
        void glDrawTransparent() const override;
    };

}
#endif
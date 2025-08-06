#ifndef DROPPED_ITEM_H
#define DROPPED_ITEM_H
#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "Item.h"
#include "Message.h"
#include "Texture.h"
namespace MyCraft {
    class DropItemManage: public MyBase3D::Controller3D, public MyBase::Port {
    public:
        DropItemManage();
        ~DropItemManage();
        DropItemManage(const DropItemManage&) = delete;
        DropItemManage& operator=(const DropItemManage&) const = delete; 

        void add(const BlockCatogary& item, const glm::vec3& position);
        void remove(const int& index);
    protected:
        bool handle(GLFWwindow*) override;
    private:
        std::vector<glm::mat4>  __state;
        std::vector<glm::vec4>  __item;
        MyBase::Clock           __rotateClock;
        MyBase::Texture         __blockTexture;
        void glDraw() const override;
    };
}
#endif
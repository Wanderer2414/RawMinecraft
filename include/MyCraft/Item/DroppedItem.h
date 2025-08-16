#ifndef DROPPED_ITEM_H
#define DROPPED_ITEM_H
#include "Block.h"
#include "Clock.h"
#include "Controller3D.h"
#include "DrawingCenter.h"
#include "Item.h"
#include "Message.h"
#include "Recipe.h"
#include "Texture.h"
namespace MyCraft {
    class DropItemManage: public MyBase3D::Controller3D, public MyBase::Port {
    public:
        DropItemManage();
        ~DropItemManage();
        DropItemManage(const DropItemManage&) = delete;
        DropItemManage& operator=(const DropItemManage&) const = delete; 
        std::vector<std::pair<glm::vec3,RecipeSlot>> getNearItem(const glm::vec3& position);
        void add(const BlockCatogary& item, const unsigned char& count, const glm::vec3& position);
        void remove(const int& index);
        friend class FallItemCommand;
    protected:
        bool handle(GLFWwindow*) override;
    private:
        int __currentFall;
        BlockDrawingStorage              __normal;
        std::vector<unsigned char>   __count;
        MyBase::Clock               __rotateClock;
        MyBase::Texture             __blockTexture;
        void glDraw() const override;
    };

    class DropItemMessage: public MyBase::Message {
    public:
        DropItemMessage(const ItemType& type, const int& count, const glm::vec3& position);
        ~DropItemMessage();

        const ItemType type;
        const int count;
        const glm::vec3 position;
        MyBase::MessageType getType() const override;
    private:
    };
    class DropItemCommand: public MyBase::Command {
    public:
        DropItemCommand(DropItemManage& manage);
        ~DropItemCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        DropItemManage& manage;
    };
    class LootItemByMoveCommand: public MyBase::Command {
    public:
        LootItemByMoveCommand(DropItemManage& manage);
        ~LootItemByMoveCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        DropItemManage& manage;
    };

    class LootItemByJumpCommand: public MyBase::Command {
    public:
        LootItemByJumpCommand(DropItemManage& manage);
        ~LootItemByJumpCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        DropItemManage& manage;
    };

    class FallItemCommand: public MyBase::Command {
    public:
        FallItemCommand(DropItemManage& manage);
        ~FallItemCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        DropItemManage& __manage;
    };
}
#endif
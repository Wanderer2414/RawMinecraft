#ifndef INVENTORY_H
#define INVENTORY_H
#include "Container2D.h"
#include "Controller2D.h"
#include "Texture.h"
namespace MyCraft {
    class Inventory: public MyBase::Container2D {
    public:
        Inventory();
        ~Inventory();
        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) const = delete; 

        bool setHover(const glm::vec2& position) override;
        glm::vec2 getPosition() const override;
        glm::vec2 getSize() const override;
    protected:
    private:
        bool contains(const glm::vec2& position) const override;
        void glDraw() const override;
        glm::vec2 getHelmetPosition() const;
        glm::vec2 getChestplatePosition() const;
        glm::vec2 getLeggingsPosition() const;
        glm::vec2 getBootsPosition() const;
        glm::vec2 getLeftHandPosition() const;
        glm::vec2 getRecipiPosition(const glm::ivec2& offfset) const;
        glm::vec2 getStoragePosition(const glm::ivec2& offset) const;
        glm::vec2 getToolbarPosition(const int& n) const;
        MyBase::Texture __chosenTexture, __inventoryTexture;
    };
}
#endif
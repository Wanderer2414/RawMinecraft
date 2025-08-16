#include "HealthBar.h"
#include "Container2D.h"
#include "ControlCenter.h"
#include "Message.h"
#include "Texture.h"

namespace MyCraft {
    HealthBar::HealthBar(): __width(0) {
        MyBase::Texture fullHeightTexture,noHealthTexture;
        fullHeightTexture.load("assets/images/FullHeart.png");
        noHealthTexture.load("assets/images/NoHeart.png");
        __fullHealthTexture.setTexture(fullHeightTexture);
        __noHealthTexture.setTexture(noHealthTexture);

        glm::vec2 size(0.15f/MyBase::ControlCenter::getInstance().GetWindowRatio()*1110.f/120.f, 0.15);

        __fullHealthTexture.setTextureImportPosition({0,0});
        __fullHealthTexture.setTextureImportSize({1,1});

        float ratio = fullHeightTexture.getSize().x*1.f/fullHeightTexture.getSize().y;
        __width = 0.04*ratio/MyBase::ControlCenter::getInstance().GetWindowRatio();
        __fullHealthTexture.setTextureExportPosition({-size.x/2.f, -0.82});
        __fullHealthTexture.setTextureExportSize({__width,0.04});

        __noHealthTexture.setTextureImportPosition({0, 0});
        __noHealthTexture.setTextureImportSize({1, 1});

        __noHealthTexture.setTextureExportPosition({-size.x/2.f, -0.82});
        __noHealthTexture.setTextureExportSize({__width,0.04});
        
        insert(&__fullHealthTexture);
        insert(&__noHealthTexture);
        add(new UpdateHealthBarCommand(*this));
    }
    HealthBar::~HealthBar() {}


    bool HealthBar::contains(const glm::vec2& position) const {
        return false;
    }

    glm::vec2 HealthBar::getPosition() const {
        return glm::vec2(0,0);
    }
    glm::vec2 HealthBar::getSize() const {
        return glm::vec2(0,0);
    }
    void HealthBar::updateHeight(const float& percent) {
        __fullHealthTexture.setTextureImportSize({percent,1});
        __fullHealthTexture.setTextureExportSize({percent*__width,0.04});
    }
    void HealthBar::glDraw() const {}

    void HealthBar::glDrawTransparent() const {
        MyBase::Container2D::glDraw();
    }

    UpdateHealthBarMessage::UpdateHealthBarMessage(const float& p): percent(p) {}
    UpdateHealthBarMessage::~UpdateHealthBarMessage() {}

    MyBase::MessageType UpdateHealthBarMessage::getType() const {
        return MyBase::UpdateHealth;
    }

    UpdateHealthBarCommand::UpdateHealthBarCommand(HealthBar& bar): __bar(bar) {}
    UpdateHealthBarCommand::~UpdateHealthBarCommand() {}
    
    MyBase::MessageType UpdateHealthBarCommand::getType() const {
        return MyBase::UpdateHealth;
    }
    
    void UpdateHealthBarCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        UpdateHealthBarMessage* package = (UpdateHealthBarMessage*)message;
        __bar.updateHeight(package->percent);
    }
}
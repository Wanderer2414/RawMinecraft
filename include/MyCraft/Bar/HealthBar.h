#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H
#include "Container2D.h"
#include "Controller2D.h"
#include "Message.h"
#include "Texture.h"
namespace MyCraft {
    class HealthBar: public MyBase::Container2D, public MyBase::Port {
    public:
        HealthBar();
        ~HealthBar();

        bool contains(const glm::vec2& position) const override;
        void updateHeight(const float& percent);
        virtual glm::vec2       getPosition() const override,
                                getSize() const override;
    private:
        MyBase::TextureViewer __fullHealthTexture, __noHealthTexture;
        float __width;

        void glDraw() const             override;
        void glDrawTransparent() const  override;
    };
    class UpdateHealthBarMessage: public MyBase::Message {
    public:
        UpdateHealthBarMessage(const float& percent);
        ~UpdateHealthBarMessage();

        const float percent;
        MyBase::MessageType getType() const override;
    };
    class UpdateHealthBarCommand: public MyBase::Command {
    public:
        UpdateHealthBarCommand(HealthBar& bar);
        ~UpdateHealthBarCommand();
        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        HealthBar& __bar;
    };
}
#endif
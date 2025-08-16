#ifndef HEALTH_MODULE_H
#define HEALTH_MODULE_H
#include "Message.h"
namespace MyCraft {
    class HealthModule {
    public:
        HealthModule(const unsigned int& max_health);
        ~HealthModule();
        bool isDead() const;
        bool isFullHealth() const;
        float getHealthPercent() const;
        unsigned char getHealth() const;
        void damage(const unsigned int&);
        void health(const unsigned int&);
    protected:
        virtual void __dead() = 0;
        virtual void __damage() = 0;
        virtual void __heal() = 0;
    private:
        unsigned int __health, __maxHealth;
    };

    class DamageMessage: public MyBase::Message {
    public:
        DamageMessage(const unsigned int& height);
        ~DamageMessage();

        const unsigned int offset;

        MyBase::MessageType getType() const override;
    };

    class DamageCommand: public MyBase::Command {
    public:
        DamageCommand(HealthModule& health);
        ~DamageCommand();

        MyBase::MessageType getType() const override;
        void execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) override;
    private:
        HealthModule& __health;
    };
}
#endif
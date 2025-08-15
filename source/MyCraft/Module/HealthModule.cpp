#include "HealthModule.h"
#include "Message.h"

namespace MyCraft {
    HealthModule::HealthModule(const unsigned int& max_health): __health(max_health), __maxHealth(max_health) {}
    HealthModule::~HealthModule() {}
        
    bool HealthModule::isDead() const {
        return (__health == 0);
    }
    bool HealthModule::isFullHealth() const {
        return (__maxHealth == __health);
    }
    unsigned char HealthModule::getHealth() const {
        return __health;
    }
    float HealthModule::getHealthPercent() const {
        return __health*1.0f/__maxHealth;
    }
    void HealthModule::damage(const unsigned int& h) {
        if (h>__health) {
            __health = 0;
            __damage();
            __dead();
        }
        else {
            __health -= h;
            __damage();
        }
    }
    void HealthModule::health(const unsigned int& h) {
        if (__maxHealth - __health >= h) __health += h;
        else __health = __maxHealth;
        __heal();
    }

    
    DamageMessage::DamageMessage(const unsigned int& h): offset(h) {}
    DamageMessage::~DamageMessage() {}

    MyBase::MessageType DamageMessage::getType() const {
        return MyBase::Damage;
    }

    DamageCommand::DamageCommand(HealthModule& health): __health(health) {}
    DamageCommand::~DamageCommand() {}

    MyBase::MessageType DamageCommand::getType() const {
        return MyBase::Damage;
    }
    void DamageCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        DamageMessage* package = (DamageMessage*)message;
        __health.damage(package->offset);
    }
}
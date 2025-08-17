#ifndef NPC_CONTROLLER_H
#define NPC_CONTROLLER_H
#include "ModelController.h"
#include "Clock.h"
#include "Message.h"
#include <glm/glm.hpp>
#include <random>
#include "ModelStorage.h"

namespace MyCraft {
    enum class NpcType{
        COW,
        WOLF,
        CHICKEN
    };
    class NPCController : public ModelController, public MyBase::Port {
        public:
            NPCController();
            ~NPCController();
            void glDraw() const override;
            virtual void performAction() = 0;
            glm::mat4x3 getShape() const override;
        
        private:
            bool            __isRun, __isDrawable,
                            __isLeftAttack, __isRightAttack,
                            __isCrouch;
            float           __runTime, __handTime, __speed;
            glm::vec3       __position, __diagonal;
            glm::vec3       __direction, __eye_direction;
            std::vector<glm::mat4> __animation;
            MyBase::Clock   __animationClock,
                            __runCooldown,
                            __attack__cooldown;
            glm::vec3       __toAbsoluteCoordinate(const glm::vec3& dir) const;
            void            update() override;
    };
}
#endif
#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H
#include "Clock.h"
#include "GLFW/glfw3.h"
#include "Model.h"
namespace MyCraft {
    class PlayerModel: public Model {
    public: 
        PlayerModel();
        ~PlayerModel();
        bool        handle(GLFWwindow* window) override;
        glm::vec3   getPosition() const override,
                    getDirection() const;
        void        move(const glm::vec3& delta) override,
                    rotate(const float& angle) override,
                    rotate(const glm::vec3& dir),
                    rightAttack(),
                    leftAttack(),
                    see(const glm::vec3& dir) override,
                    seeRotate(const float& angle),
                    glDraw() const override;
        
        const glm::mat4x3& getBottomRec() const override;
        glm::vec2 getZRange() const override;
        
        
    private:
        bool            __isRun,
                        __isLeftAttack, __isRightAttack,
                        __isCrouch;
        float           __runTime, __handTime, __speed;
        glm::vec3       __postition;
        glm::vec3       __direction, __eye_direction;
        std::vector<glm::mat4> __animation;
        MyBase::Clock   __animationClock, 
                        __behaviourClock,
                        __attack__cooldown;
    };
}
#endif
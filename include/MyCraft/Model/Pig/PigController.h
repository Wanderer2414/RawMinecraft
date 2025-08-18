#ifndef PIG_CONTROLLER_H
#define PIG_CONTROLLER_H
#include "ModelController.h"
#include "Pig/PigModel.h"
namespace MyCraft {
    namespace Pig {
        class PigController: public ModelController, private Pig::Model {
        public:
            PigController();
            ~PigController();

            void    see(const glm::vec3& dir) override,
                    move(const glm::vec3& dir) override,
                    rotate(const glm::vec3& angle) override;

            void    update() override;
            glm::vec3 getPosition() const override;
            glm::mat4x3 getShape() const override;
        private:
            bool __isChanged;
            float __speed;
            bool handle(GLFWwindow* window) override;
            void glDraw() const override;

            void    __see(const glm::vec3& dir) override,
                    __move(const glm::vec3& dir) override,
                    __rotate(const glm::vec3& angle) override;
        };
    }
}
#endif
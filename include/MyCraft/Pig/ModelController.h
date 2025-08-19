#ifndef PIG_CONTROLLER_H
#define PIG_CONTROLLER_H
#include "Model/ModelController.h"
#include "Pig/Model.h"
namespace MyCraft {
    namespace Pig {
        class Controller: public ModelController, private Pig::Model {
        public:
            Controller();
            ~Controller();

            void    see(const glm::vec3& dir) override,
                    move(const glm::vec3& dir) override,
                    look(const glm::vec3& position) override,
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
                    __look(const glm::vec3& pos) override,
                    __rotate(const glm::vec3& angle) override;
        };
    }
}
#endif
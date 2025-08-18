#ifndef PIG_MODEL_H
#define PIG_MODEL_H
#include "PlayerModel.h"
#include "Texture.h"
namespace MyCraft {
    namespace Pig {
        class Model: public GLTFModel {
        public:
            Model();
            ~Model();
        protected:

            glm::mat4x3 getShape()              const   override;
            glm::vec3   getPosition()           const   override;
            void look(const glm::vec3& position)        override;
            bool attack()                               override;
            void see(const glm::vec3&)                  override;
            void move(const glm::vec3& direction)       override;
            void rotate(const glm::vec3& direction)     override;
            void setPosition(const glm::vec3& position) override;
            bool apply()                                override;
            void draw() const;
        private:
            MyBase::Texture __pigTexture;
        };
    }
}
#endif
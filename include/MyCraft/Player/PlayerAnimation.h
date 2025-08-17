#ifndef PLAYER_ANIMATION_H
#define PLAYER_ANIMATION_H
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
#include "Global.h"
namespace MyCraft {
    namespace Player {
        class WalkAnimation: public GLTFAnimation {
        public:
            WalkAnimation(const tinygltf::Model& model);
            ~WalkAnimation();
            WalkAnimation(const WalkAnimation&) = delete;
            WalkAnimation& operator=(const WalkAnimation&) const = delete; 
            void apply(GLTFStaticMesh& set, const float& percent);
        protected:
        private:
        };
    }
}
#endif
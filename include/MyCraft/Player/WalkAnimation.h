#ifndef PLAYER_WALK_ANIMATION_h
#define PLAYER_WALK_ANIMATION_h
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Player {
        class WalkAnimation {
        public:
            WalkAnimation(GLTFAnimation& animation);
            ~WalkAnimation();
            WalkAnimation(const WalkAnimation&) = delete;
            WalkAnimation& operator=(const WalkAnimation&) const = delete; 
            void apply(GLTFStaticMesh::SetNode& node, const float& percent);
        protected:
        private:
            GLTFAnimation& __animation;
        };
    }
}
#endif
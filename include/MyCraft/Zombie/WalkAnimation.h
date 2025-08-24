#ifndef ZOMBIE_WALK_ANIMATION_H
#define ZOMBIE_WALK_ANIMATION_H
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Zombie {
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
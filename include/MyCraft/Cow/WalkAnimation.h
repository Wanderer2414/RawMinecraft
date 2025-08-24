#ifndef COW_WALK_ANIMATION_H
#define COW_WALK_ANIMATION_H
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Cow {
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
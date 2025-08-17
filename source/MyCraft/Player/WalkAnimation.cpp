#include "WalkAnimation.h"

namespace MyCraft {
    namespace Player {
        WalkAnimation::WalkAnimation(GLTFAnimation& animation): __animation(animation) {}
        WalkAnimation::~WalkAnimation() {}
        void WalkAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[1] = __animation.get(1, percent);
            node[5] = __animation.get(5, percent);
            node[7] = __animation.get(7, percent);
            node[9] = __animation.get(9, percent);
        }
    }
}
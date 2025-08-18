#include "WalkAnimation.h"

namespace MyCraft {
    namespace Player {
        WalkAnimation::WalkAnimation(GLTFAnimation& animation): __animation(animation) {}
        WalkAnimation::~WalkAnimation() {}
        void WalkAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[1] = __animation.get(1, percent);
            node[3] = __animation.get(3, percent);
            node[7] = __animation.get(7, percent);
            node[9] = __animation.get(9, percent);
        }
    }
}
#include "WalkAnimation.h"

namespace MyCraft {
    namespace Pig {
        WalkAnimation::WalkAnimation(GLTFAnimation& animation): __animation(animation) {}
        WalkAnimation::~WalkAnimation() {}

        void WalkAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[6] = __animation.get(6, percent);
            node[8] = __animation.get(8, percent);
            node[10] = __animation.get(10, percent);
            node[12] = __animation.get(12, percent);
        }
    }
}
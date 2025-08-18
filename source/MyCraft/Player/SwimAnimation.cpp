#include "SwimAnimation.h"

namespace MyCraft {
    namespace Player {
        SwimAnimation::SwimAnimation(GLTFAnimation& animation): __animation(animation) {}
        SwimAnimation::~SwimAnimation() {}
        void SwimAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            for (int i = 0; i<12; i++) node[i] = __animation.get(i, percent);
        }
    }
}
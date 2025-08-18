#include "CrouchAnimation.h"

namespace MyCraft {
    namespace Player {
        CrouchAnimation::CrouchAnimation(GLTFAnimation& animation): __animation(animation) {}
        CrouchAnimation::~CrouchAnimation() {}
        void CrouchAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[1] = __animation.get(1, percent);
            node[3] = __animation.get(3, percent);
            node[5] = __animation.get(1, percent);
            node[11] = __animation.get(11, percent);
        }
    }
}
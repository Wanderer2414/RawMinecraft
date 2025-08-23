#include "Cow/WalkAnimation.h"

namespace MyCraft {
    namespace Cow {
        WalkAnimation::WalkAnimation(GLTFAnimation& animation): __animation(animation) {}
        WalkAnimation::~WalkAnimation() {}

        void WalkAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[9] = __animation.get(9, percent);
            node[11] = __animation.get(11, percent);
            node[13] = __animation.get(13, percent);
            node[15] = __animation.get(15, percent);
        }
    }
}
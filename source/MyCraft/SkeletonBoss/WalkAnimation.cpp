#include "SkeletonBoss/WalkAnimation.h"

namespace MyCraft{
    namespace SkeletonBoss{
        WalkAnimation::WalkAnimation(GLTFAnimation& animation): __animation(animation) {}
        WalkAnimation::~WalkAnimation() {}

        void WalkAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[61] = __animation.get(61, percent);
            node[54] = __animation.get(54, percent);
            node[84] = __animation.get(84, percent);
        }
    }
}
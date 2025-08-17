#include "AttackAnimation.h"
namespace MyCraft {
    namespace Player {

        AttackAnimation::AttackAnimation(GLTFAnimation& animation):__animation(animation) {}
        AttackAnimation::~AttackAnimation() {}

        void AttackAnimation::apply(GLTFStaticMesh::SetNode& node, const float& percent) {
            node[1] = __animation.get(1, percent);
            node[5] = __animation.get(5, percent);
        }
    }
}
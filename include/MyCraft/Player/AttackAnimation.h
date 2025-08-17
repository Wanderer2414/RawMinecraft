#ifndef ATACK_ANIMATION_H
#define ATACK_ANIMATION_H
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Player {
        class AttackAnimation {
        public:
            AttackAnimation(GLTFAnimation& animation);
            ~AttackAnimation();
            AttackAnimation(const AttackAnimation&) = delete;
            AttackAnimation& operator=(const AttackAnimation&) const = delete; 
            void apply(GLTFStaticMesh::SetNode& node, const float& percent);
        protected:
        private:
            GLTFAnimation& __animation;
        };
    }
}
#endif
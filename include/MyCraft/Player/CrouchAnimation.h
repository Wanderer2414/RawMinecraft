#ifndef PLAYER_CROUCH_ANIMATION
#define PLAYER_CROUCH_ANIMATION
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Player {
        class CrouchAnimation {
        public:
            CrouchAnimation(GLTFAnimation& animation);
            ~CrouchAnimation();
            CrouchAnimation(const CrouchAnimation&) = delete;
            CrouchAnimation& operator=(const CrouchAnimation&) const = delete; 
            void apply(GLTFStaticMesh::SetNode& node, const float& percent);
        protected:
        private:
            GLTFAnimation& __animation;
        };
    }
}
#endif
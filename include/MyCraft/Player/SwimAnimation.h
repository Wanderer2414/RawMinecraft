#ifndef SWIM_ANIMATION_H
#define SWIM_ANIMATION_H
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft {
    namespace Player {
        class SwimAnimation {
        public:
            SwimAnimation(GLTFAnimation& animation);
            ~SwimAnimation();
            SwimAnimation(const SwimAnimation&) = delete;
            SwimAnimation& operator=(const SwimAnimation&) const = delete; 
            void apply(GLTFStaticMesh::SetNode& node, const float& percent);
        protected:
        private:
            GLTFAnimation& __animation;
        };
    }
}
#endif
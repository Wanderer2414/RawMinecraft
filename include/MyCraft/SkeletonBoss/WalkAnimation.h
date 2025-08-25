#pragma once
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
namespace MyCraft{
    namespace SkeletonBoss{
        class WalkAnimation{
        public:
            WalkAnimation(GLTFAnimation& animation);
            ~WalkAnimation();
            WalkAnimation(const WalkAnimation&) = delete;
            WalkAnimation& operator=(const WalkAnimation&) const = delete; 

            void apply(GLTFStaticMesh::SetNode& node, const float& percent);
        private:
            GLTFAnimation& __animation;
        };
    }
}
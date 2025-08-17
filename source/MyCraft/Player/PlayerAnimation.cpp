#include "PlayerAnimation.h"
#include "GLTFAnimation.h"
#include "GLTFMesh.h"
#include "Global.h"
namespace MyCraft {
    namespace Player {
        WalkAnimation::WalkAnimation(const tinygltf::Model& model): GLTFAnimation(model, model.animations[0]) {}
        WalkAnimation::~WalkAnimation() {}

        void WalkAnimation::apply(GLTFStaticMesh& set, const float& percent) {
            set.States()[1] = get(1, percent);
            set.States()[5] = get(5, percent);
            set.States()[7] = get(7, percent);
            set.States()[9] = get(9, percent);
        }
    }
}
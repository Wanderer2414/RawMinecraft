#include "GLTFAnimation.h"
#include "GLTFNodeAnimation.h"
#include "Global.h"

namespace MyCraft {
    GLTFAnimation::GLTFAnimation(const tinygltf::Model& model, const tinygltf::Animation& animation) {
        __subAnimations.resize(animation.channels.size());
        for (int i = 0; i<animation.channels.size(); i++) {
            const tinygltf::AnimationChannel& channel = animation.channels[i];
            if (channel.target_path == "rotation") {
                __subAnimations[i] = new NodeRotation(model, animation, channel);
                __indices[channel.target_node] = i;
            }
        }
    }
    GLTFAnimation::~GLTFAnimation() {}
    glm::mat4 GLTFAnimation::get(const float& node, const float& p) {
        float percent = p;
        while (percent>1) percent--;
        while (percent<0) percent++;
        if (__indices.contains(node)) return (*__subAnimations[__indices[node]])[p];
        return glm::mat4(1);
    }
}
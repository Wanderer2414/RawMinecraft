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
                __indices[channel.target_node].push_back(i);
            }
            else if (channel.target_path == "translation") {
                __subAnimations[i] = new NodeTranslation(model, animation,channel);
                auto& vec = __indices[channel.target_node];
                vec.push_back(i);
                std::swap(vec.front(), vec.back());
            }
        }
    }
    GLTFAnimation::~GLTFAnimation() {}
    glm::mat4 GLTFAnimation::get(const float& node, const float& p) {
        float percent = p;
        while (percent>1) percent--;
        while (percent<0) percent++;
        glm::mat4 final(1);
        if (__indices.contains(node)) 
            for (const int& i:__indices[node]) final=final*(*__subAnimations[i])[p];
        return final;
    }
}
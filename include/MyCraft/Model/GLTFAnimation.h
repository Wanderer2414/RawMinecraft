#ifndef GLTF_ANIMATION_H
#define GLTF_ANIMATION_H
#include "GLTFNodeAnimation.h"
#include "Global.h"
namespace MyCraft {
    class GLTFAnimation {
    public:
        GLTFAnimation(const tinygltf::Model& model, const tinygltf::Animation& animation);
        ~GLTFAnimation();
        GLTFAnimation& operator=(const GLTFAnimation&) const = delete; 
        GLTFAnimation(const GLTFAnimation&) = delete;
        glm::mat4 get(const float& node, const float& percent);
    private:
        std::vector<GLTFNodeAnimation*> __subAnimations;
        std::map<int, int> __indices;
    };
}
#endif
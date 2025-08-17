#include "GLTFNodeAnimation.h"
#include "Global.h"

namespace MyCraft {

    GLTFNodeAnimation::~GLTFNodeAnimation() {}
    NodeRotation::NodeRotation(const tinygltf::Model& model, const tinygltf::Animation& animation, const tinygltf::AnimationChannel& channel): __root(0), __cur(0) {
        if (channel.target_path != "rotation") 
            throw std::runtime_error("Wrong type of animation");
        const tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
        const tinygltf::Accessor& TimeAccessor = model.accessors[sampler.input];
        const tinygltf::Accessor& ValueAccessor = model.accessors[sampler.output];
        const tinygltf::BufferView& TimeView = model.bufferViews[TimeAccessor.bufferView];
        const tinygltf::BufferView& ValueView = model.bufferViews[ValueAccessor.bufferView];
        
        float* time = (float*)(model.buffers[TimeView.buffer].data.data() + TimeView.byteOffset + TimeAccessor.byteOffset);
        glm::quat* value = (glm::quat*)(model.buffers[ValueView.buffer].data.data() + ValueView.byteOffset + ValueAccessor.byteOffset);

        float length = time[TimeAccessor.count-1];

        for (int i = TimeAccessor.count-1; i>=0; i--) {
            __cur = new Node(__cur);
            if (!__root) __root = __cur;
            __cur->percent = time[i]/length;
            __cur->state = value[i];
        }
        __root->next = __cur;
        __cur = __root;
    }
    NodeRotation::~NodeRotation() {
        delete __root;
        __root = __cur = 0;
    }
    glm::mat4 NodeRotation::operator[](const float& p) {
        float percent = p;
        while (percent>1) percent--;
        while (percent<0) percent++;
        if (percent < __cur->percent) __cur = __root;
        while (percent>__cur->next->percent && __cur->next != __root) __cur = __cur->next;
        float ratio = 0;
        if (__cur->next == __root)
            ratio = (percent-__cur->percent)/(1 - __cur->percent);
        else ratio = (percent- __cur->percent)/(__cur->next->percent - __cur->percent);
        return glm::toMat4(glm::normalize(glm::slerp(__cur->state, __cur->next->state, ratio)));
    }
    NodeRotation::Node::Node(Node* n): next(n) {}
    NodeRotation::Node::~Node() {
        Node* tmp = next;
        next = 0;
        if (tmp) delete tmp;
    }
}
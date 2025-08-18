#ifndef GLTF_NODE_ANIMATION_H
#define GLTF_NODE_ANIMATION_H
#include "Global.h"

namespace MyCraft {
    class GLTFNodeAnimation {
    public:
        GLTFNodeAnimation() = default;
        virtual ~GLTFNodeAnimation();
        GLTFNodeAnimation(const GLTFNodeAnimation&) = delete;
        GLTFNodeAnimation& operator=(const GLTFNodeAnimation&) const = delete; 
        virtual glm::mat4 operator[](const float& percent) = 0;
    protected:
    private:
    };

    class NodeRotation: public GLTFNodeAnimation {
    public:
        NodeRotation(const tinygltf::Model& model, const tinygltf::Animation& animation, const tinygltf::AnimationChannel& channel);
        ~NodeRotation();
        NodeRotation(const NodeRotation&) = delete;
        NodeRotation& operator=(const NodeRotation&) const = delete; 
        glm::mat4 operator[](const float& percent) override;
    protected:
    private:
        struct Node {
            float       percent;
            glm::quat   state;
            Node*       next;
            Node(Node*);
            ~Node();
        };
        Node* __root, *__cur;
    };
    class NodeTranslation: public GLTFNodeAnimation {
    public:
        NodeTranslation(const tinygltf::Model& model, const tinygltf::Animation& animation, const tinygltf::AnimationChannel& channel);
        ~NodeTranslation();
        NodeTranslation(const NodeRotation&) = delete;
        NodeTranslation& operator=(const NodeRotation&) const = delete; 
        glm::mat4 operator[](const float& percent) override;
    protected:
    private:
        struct Node {
            float       percent;
            glm::vec3   state;
            Node*       next;
            Node(Node*);
            ~Node();
        };
        Node* __root, *__cur;  
    };
}
#endif
#ifndef GLTF_MESH
#define GLTF_MESH
#include "FlyweightStorage.h"
#include "GLTFAnimation.h"
#include "Global.h"

namespace MyCraft {
    class GLTFStaticMesh: public MyBase::FlyWeightCore {
    public:
        class SetNode {
        public:
            SetNode(const int& size);
            ~SetNode();
            SetNode(const SetNode&) = delete;
            SetNode& operator=(const SetNode&) const = delete; 
            glm::mat4& operator[](const int& index);
            const glm::mat4& operator[](const int& index) const;
            void reset();
            friend class GLTFStaticMesh;
        protected:
        private:
            glm::mat4* __states;
            unsigned int __size;
        };
        ~GLTFStaticMesh();
        GLTFStaticMesh(const GLTFStaticMesh&)   = delete;
        GLTFStaticMesh(GLTFStaticMesh&&)        = delete;
        GLTFStaticMesh& operator=(const GLTFStaticMesh&) = delete;

        int getNodeSize() const;
        GLTFAnimation& Animations(const std::string& name);
        friend class GLTFModel;
    private:
        int     __nodeSize;
        struct Node {
            std::vector<Node*> children;
            glm::vec3 translation;
            int node;
            ~Node();
        };
        Node* __root;
        GLTFStaticMesh(const tinygltf::Model& src);
        void BindModel(const tinygltf::Model& model);
        void BindBuffer(const tinygltf::Model& model);
        void BindNodes(const int& index, Node*& root, const tinygltf::Model& model);
        void BindMesh(const tinygltf::Model& model, const int& index);
        void DrawModelNodes(Node* root, const glm::mat4& state, const SetNode& node) const;
        void DrawMesh(const int& index) const;
        void LoadAnimation(const tinygltf::Model& model);
        void Draw(const SetNode& nodes) const;
        std::vector<GLuint> __VAO;
        std::vector<GLuint> __ebos;
        std::map<std::string,GLTFAnimation*> __animation;
        GLuint __state;
    };

}
#endif
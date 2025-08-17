#ifndef GLTF_MESH
#define GLTF_MESH
#include "FlyweightStorage.h"
#include "Global.h"

namespace MyCraft {
    class GLTFStaticMesh: public MyBase::FlyWeightCore {
    public:
        class SetNode {
        public:
            SetNode(const SetNode&) = delete;
            SetNode& operator=(const SetNode&) const = delete; 
            glm::mat4& operator[](const int& index);
            const glm::mat4& operator[](const int& index) const;
            friend class GLTFStaticMesh;
        protected:
        private:
            glm::mat4* __states;
            SetNode(const int& size);
            ~SetNode();
        };
        ~GLTFStaticMesh();
        GLTFStaticMesh(const GLTFStaticMesh&)   = delete;
        GLTFStaticMesh(GLTFStaticMesh&&)        = delete;
        GLTFStaticMesh& operator=(const GLTFStaticMesh&) = delete;

        SetNode& States();
        void Draw() const;
        friend class GLTFModel;
    private:
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
        void DrawModelNodes(Node* root, const glm::mat4& state) const;
        void DrawMesh(const int& index) const;
        SetNode* __nodes;
        std::vector<GLuint> __VAO;
        std::vector<GLuint> __ebos;
        GLuint __state;
    };

}
#endif
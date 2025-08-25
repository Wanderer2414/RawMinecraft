#include "GLTFMesh.h"
#include "GLTFAnimation.h"
#include "Global.h"
#include "ShaderStorage.h"

namespace MyCraft {
    GLTFStaticMesh::Node::~Node() {
        for (int i = 0; i<children.size(); i++) delete children[i];
        children.clear();
    }
    glm::mat4& GLTFStaticMesh::SetNode::operator[](const int& index) {
        return __states[index];
    }
    int GLTFStaticMesh::getNodeSize() const {
        return __nodeSize;
    }
    const glm::mat4& GLTFStaticMesh::SetNode::operator[](const int& index) const {
        return __states[index];
    }
    void GLTFStaticMesh::SetNode::reset() {
        for (int i = 0; i<__size; i++) __states[i] = glm::mat4(1);
    }
    GLTFStaticMesh::SetNode::SetNode(const int& size):__size(size) {
        __states = new glm::mat4[size];
        for (int i = 0; i<size; i++) __states[i] = glm::mat4(1);
    }
    GLTFStaticMesh::SetNode::~SetNode() {
        delete[] __states;
    }
    GLTFAnimation& GLTFStaticMesh::Animations(const std::string& name) {
        return *__animation.at(name);
    }

    GLTFStaticMesh::GLTFStaticMesh(const tinygltf::Model& model):__VAO(0) {
        BindModel(model);
        LoadAnimation(model);

        glGenBuffers(1, &__state);
        glBindBuffer(GL_UNIFORM_BUFFER, __state);
        glm::mat4 mat(1);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &mat, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __state);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    GLTFStaticMesh::~GLTFStaticMesh() {
        glDeleteVertexArrays(__VAO.size(), __VAO.data());
        glDeleteBuffers(1, &__state);
        glDeleteBuffers(__ebos.size(), __ebos.data());
        delete __root;
        for (auto& i:__animation) delete i.second;
    }
    void GLTFStaticMesh::BindModel(const tinygltf::Model& model) {
        __VAO.resize(model.meshes.size());
        glGenVertexArrays(model.meshes.size(), __VAO.data());
        BindBuffer(model);
        for (int i = 0; i<model.meshes.size(); i++) BindMesh(model, i);
        //Get state count
        __nodeSize = model.nodes.size();
        BindNodes(model.nodes.size()-1, __root, model);
        //Bind parent state
        glBindVertexArray(0);
    }
    void GLTFStaticMesh::BindBuffer(const tinygltf::Model& model) {
        __ebos.resize(model.bufferViews.size());
        glGenBuffers(model.bufferViews.size(), __ebos.data());
        for(int i = 0; i < model.bufferViews.size(); ++i) {
            const tinygltf::BufferView & bufferView = model.bufferViews[i];
            if(bufferView.target){
                glBindBuffer(bufferView.target, __ebos[i]);
                const tinygltf::Buffer & buffer = model.buffers[bufferView.buffer];
                glBufferData(bufferView.target, bufferView.byteLength, buffer.data.data() + bufferView.byteOffset, GL_STATIC_DRAW);
                glBindBuffer(bufferView.target, 0);
            }
        }
    }

    void GLTFStaticMesh::BindNodes(const int& index, Node*& root, const tinygltf::Model& model) {
        root = new Node();
        if (model.nodes[index].translation.size()) {
            const double* offset = model.nodes[index].translation.data();
            root->translation = glm::vec3(offset[0], -offset[2], offset[1]);
        }
        else root->translation = glm::vec3(0);
        if (model.nodes[index].children.size()) {
            root->node = index;
            root->children.resize(model.nodes[index].children.size(), 0);
            for (int i = 0; i<model.nodes[index].children.size(); i++)
                BindNodes(model.nodes[index].children[i], root->children[i], model);
        }
        else root->node = model.nodes[index].mesh;
    }

    void GLTFStaticMesh::BindMesh(const tinygltf::Model& model, const int& index) {
        const tinygltf::Mesh& mesh = model.meshes[index];
        glBindVertexArray(__VAO[index]);
        
        const tinygltf::Primitive& primitive = mesh.primitives.front();
        const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __ebos[indexAccessor.bufferView]);
        for(auto & attrib : primitive.attributes){
            const tinygltf::Accessor& accessor = model.accessors[attrib.second];
            int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, __ebos[accessor.bufferView]);

            int size = 1;
            if(accessor.type != TINYGLTF_TYPE_SCALAR){
                size = accessor.type;
            }
            int attribute = -1;
            if(attrib.first.compare("POSITION") == 0) {
                attribute = 0;
            } else if(attrib.first.compare("TEXCOORD_0") == 0) {
                attribute = 1;
            } else if(attrib.first.compare("NORMAL") == 0) {
                attribute = 2;
            }
            if(attribute >= 0){
                glEnableVertexAttribArray(attribute);
                glVertexAttribPointer(attribute, size, accessor.componentType, GL_FALSE, byteStride, (char*)NULL + accessor.byteOffset);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(0);
    }


    void GLTFStaticMesh::Draw(const SetNode& node) const {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetModelShader());
        DrawModelNodes(__root, glm::mat4(1), node);
    }

    void GLTFStaticMesh::DrawModelNodes(Node* root, const glm::mat4& state, const SetNode& nodes) const {
        glm::mat4 new_state = state;
        if (root->children.size()) {
            new_state = new_state*glm::translate(glm::mat4(1), root->translation);
            new_state *= nodes[root->node];
        }
        for(size_t i = 0; i < root->children.size(); ++i) {
            DrawModelNodes(root->children[i], new_state, nodes);
        }
        if (root->children.empty()) {
            new_state *= glm::translate(glm::mat4(1), root->translation);
            glBindBuffer(GL_UNIFORM_BUFFER, __state);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &new_state);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __state);
            
            DrawMesh(root->node);
        }
    }
    void GLTFStaticMesh::LoadAnimation(const tinygltf::Model& model) {
        for (int i = 0; i<model.animations.size() ;i++) {
            __animation[model.animations[i].name] = new GLTFAnimation(model, model.animations[i]);
        }
    }

    void GLTFStaticMesh::DrawMesh(const int& index) const {
        glBindVertexArray(__VAO[index]);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
}
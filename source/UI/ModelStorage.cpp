#include "ModelStorage.h"
#include "Global.h"
#include "ShaderStorage.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include <queue>
namespace MyCraft {
    ModelStorage::ModelStorage() {
        __loadPlayerModel();
        glGenBuffers(1, &__nodeState);
        glBindBuffer(GL_UNIFORM_BUFFER, __nodeState);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __nodeState);
    }
    ModelStorage::~ModelStorage() {
        __freePlayerModel();
        glDeleteBuffers(1, &__nodeState);
    }
    std::vector<float> indices;
    void ModelStorage::__loadPlayerModel() {
        tinygltf::TinyGLTF loader;
        std::string err, warn;
        if (!loader.LoadASCIIFromFile(&__playerModel, &err, &warn, "assets/models/Playermodel.gltf")) {
            std::cout << "Failed to load player model" << std::endl;
            exit(0);
        }
        const tinygltf::Scene& scenes = __playerModel.scenes[__playerModel.defaultScene];
        
        __playerModelPoint.resize(__playerModel.bufferViews.size());
        for (std::size_t i = 0; i<__playerModel.bufferViews.size(); i++) {
            tinygltf::BufferView& bufferView = __playerModel.bufferViews[i];
            tinygltf::Buffer& buffer = __playerModel.buffers[bufferView.buffer];
            float* data = (float*)(buffer.data.data()  + bufferView.byteOffset);

            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(bufferView.target, VBO);
            glBufferData(bufferView.target, bufferView.byteLength, data, GL_STATIC_DRAW);
            glBindBuffer(bufferView.target, 0);
            __playerModelPoint[i] = VBO;
        }
    }
    void ModelStorage::__freePlayerModel() {
        for (int i = 0; i<__playerModelPoint.size(); i++)
            glDeleteBuffers(1, &__playerModelPoint[i]);
    }
    void ModelStorage::DrawModel() {
        glUseProgram(MyBase3D::ShaderStorage::Default->getModelShader());
        tinygltf::Scene& scene = __playerModel.scenes[__playerModel.defaultScene];
        for (int i = 0; i<scene.nodes.size(); i++) {
            __drawNode(scene.nodes[i], glm::mat4(1), __playerModel);
        }
    }
    void ModelStorage::__drawNode(const int& nodeIndex, glm::mat4 offset, tinygltf::Model& model) {
        tinygltf::Node& node = model.nodes[nodeIndex];
        if (node.translation.size()) 
            offset = glm::translate(offset, glm::vec3(node.translation[0], node.translation[2], node.translation[1]));
        if (node.mesh >= 0 && node.mesh<__playerModel.meshes.size()) {
            glBindBuffer(GL_UNIFORM_BUFFER, __nodeState);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &offset);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __nodeState);
            __drawMesh(node.mesh, __playerModelPoint, __playerModel);
        }
        
        for (int i:model.nodes[nodeIndex].children) 
            __drawNode(i, offset, model);
    }
    void ModelStorage::__drawMesh(const int& meshIndex, std::vector<GLuint>& set, tinygltf::Model& model) {
        tinygltf::Mesh& mesh = model.meshes[meshIndex];
        for (auto& prim:mesh.primitives) {
            tinygltf::Accessor& accessor = model.accessors[prim.attributes["POSITION"]];
            tinygltf::BufferView& view= model.bufferViews[accessor.bufferView];
            glBindBuffer(view.target, set[accessor.bufferView]);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, accessor.type, accessor.componentType, GL_FALSE, accessor.ByteStride(view), (void*)accessor.byteOffset);
            glBindBuffer(view.target, 0);
            tinygltf::Accessor& accessorIndices = model.accessors[prim.indices];
            
            tinygltf::BufferView& bufferView = model.bufferViews[accessorIndices.bufferView];
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, __playerModelPoint[accessorIndices.bufferView]);
            glDrawElements(GL_LINE_STRIP, accessorIndices.count, accessorIndices.componentType, (void*)accessorIndices.byteOffset);
        }
    }
}
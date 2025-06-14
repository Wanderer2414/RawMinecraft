#include "ModelStorage.h"
#include "Global.h"
#include "ModelLoader.h"
#include "ShaderStorage.h"
namespace MyCraft {
    ModelStorage::ModelStorage(): __playerModel("assets/models/Playermodel.gltf") {
        glGenBuffers(1, &__nodeState);
        glBindBuffer(GL_UNIFORM_BUFFER, __nodeState);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __nodeState);
    }
    ModelStorage::~ModelStorage() {
        glDeleteBuffers(1, &__nodeState);
    }
    void ModelStorage::DrawModel(std::vector<glm::mat4>& state, const ModelLoader& model) {
        glUseProgram(MyBase3D::ShaderStorage::Default->getModelShader());
        const tinygltf::Scene& scene = model.__model.scenes[model.__model.defaultScene];
        __drawNode(scene.nodes.back(), state, model);
    }
    void ModelStorage::__drawNode(const int& nodeIndex, std::vector<glm::mat4>& state, const ModelLoader& lmodel) {
        const tinygltf::Model& model = lmodel.__model;
        const tinygltf::Node& node = model.nodes[nodeIndex];
        if (model.nodes[nodeIndex].translation.size()) {
            state[nodeIndex] = glm::translate(state[nodeIndex], glm::vec3(node.translation[0], node.translation[2], node.translation[1]));
        }
        if (node.mesh >= 0 && node.mesh<model.meshes.size()) {
            glBindBuffer(GL_UNIFORM_BUFFER, __nodeState);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &state[nodeIndex]);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __nodeState);
            __drawMesh(node.mesh, lmodel);
        }
        for (int i:model.nodes[nodeIndex].children) {
            state[i] = state[nodeIndex]*state[i];
            __drawNode(i, state, lmodel);
        }
    }
    void ModelStorage::__drawMesh(const int& meshIndex, const ModelLoader& lmodel) {
        const tinygltf::Model& model = lmodel.__model;
        const tinygltf::Mesh& mesh = model.meshes[meshIndex];
        for (const auto& prim:mesh.primitives) {
            const tinygltf::Accessor& accessor = model.accessors[prim.attributes.at("POSITION")];
            const tinygltf::BufferView& view= model.bufferViews[accessor.bufferView];
            glBindBuffer(view.target, lmodel.__buffers[accessor.bufferView]);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, accessor.type, accessor.componentType, GL_FALSE, accessor.ByteStride(view), (void*)accessor.byteOffset);
            glBindBuffer(view.target, 0);
            const tinygltf::Accessor& accessorIndices = model.accessors[prim.indices];
            
            const tinygltf::BufferView& bufferView = model.bufferViews[accessorIndices.bufferView];
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lmodel.__buffers[accessorIndices.bufferView]);
            glDrawElements(GL_LINE_STRIP, accessorIndices.count, accessorIndices.componentType, (void*)accessorIndices.byteOffset);
        }
    }
    ModelLoader& ModelStorage::getPlayerModel() {
        return __playerModel;
    }
}
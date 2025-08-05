#include "NpcStorage.h"
#include "Global.h"
#include "ModelLoader.h"
#include "ShaderStorage.h"
namespace MyCraft{
    NPCStorage::NPCStorage(std::string & source): __NPCModel(source){
        glGenBuffers(1, &__nodeState);
        glBindBuffer(GL_UNIFORM_BUFFER, __nodeState);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, __nodeState);
    }

    NPCStorage::~NPCStorage(){
        glDeleteBuffers(1, &__nodeState);
    }

    void NPCStorage::DrawModel(std::vector<glm::mat4>& state, const ModelLoader& model) {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getModelShader());
        const tinygltf::Scene& scene = model.__model.scenes[model.__model.defaultScene];
        __drawNode(scene.nodes.back(), state, model);
    }
    void NPCStorage::__drawNode(const int & nodeIndex, std::vector<glm::mat4>& state, const ModelLoader & lmodel){
        const tinygltf::Node& node = lmodel.__model.nodes[nodeIndex];
        if (node.mesh >= 0) {
            __drawMesh(node.mesh, lmodel);
        }
        if (!node.children.empty()) {
            for (const int & child : node.children) {
                __drawNode(child, state, lmodel);
            }
        }
    }
    void NPCStorage::__drawMesh(const int & meshIndex, const ModelLoader & lmodel){
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

    ModelLoader & NPCStorage::getNPCModel(){
        return __NPCModel;
    }
}
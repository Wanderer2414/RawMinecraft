#include "MyCraft/ModelTest/gltf_mesh.h"

GLTFStaticMesh::GLTFStaticMesh(const char* filename, float scale){
    tinygltf::TinyGLTF loader;
    std::string error, warning;

    loader.LoadASCIIFromFile(&model, &error, & warning, filename);

    if(!warning.empty()){
        std::cout<<"Warning: "<< warning << std::endl;
    }
    if(!error.empty()){
        std::cerr<<"Error: "<< error << std::endl;
    }

    this->scale = scale;
    VAO_and_EBOs = bindModel();

}
GLTFStaticMesh::~GLTFStaticMesh() {
    glDeleteVertexArrays(1, &VAO_and_EBOs.first);

    for(auto it = VAO_and_EBOs.second.cbegin(); it != VAO_and_EBOs.second.cend();){
        glDeleteBuffers(1, &VAO_and_EBOs.second[it->first]);
        VAO_and_EBOs.second.erase(it++);
    }
}
std::pair<unsigned int, std::map<int, unsigned int>> GLTFStaticMesh::bindModel() {
    std::map<int, unsigned int> ebos;
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const tinygltf::Scene & scene = model.scenes[model.defaultScene];
    for(size_t i = 0; i < scene.nodes.size(); ++i) {
        assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
        bindModelNodes(ebos, model.nodes[scene.nodes[i]]);
    }
    glBindVertexArray(0);

    for(auto it = ebos.cbegin(); it != ebos.cend();){
        tinygltf::BufferView bufferView = model.bufferViews[it->first];
        if(bufferView.target != GL_ELEMENT_ARRAY_BUFFER){
            glDeleteBuffers(1, &ebos[it->first]);
            ebos.erase(it++);
        }
        else{
            ++it;
        }
    }
    return {vao, ebos};
}

void GLTFStaticMesh::bindMesh(std::map<int, unsigned int> & ebos, tinygltf::Mesh& mesh) {
    for(size_t i = 0; i < model.bufferViews.size(); ++i) {
        const tinygltf::BufferView & bufferView = model.bufferViews[i];
        if(bufferView.target == 0){
            continue;
        }

        const tinygltf::Buffer & buffer = model.buffers[bufferView.buffer];

        unsigned int ebo;
        glGenBuffers(1, &ebo);
        ebos[i] = ebo;
        glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

    }

    for(size_t i = 0; i < mesh.primitives.size(); ++i){
        tinygltf::Primitive primitive = mesh.primitives[i];
        tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];
        for(auto & attrib : primitive.attributes){
            tinygltf::Accessor accessor = model.accessors[attrib.second];
            int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
            glBindBuffer(GL_ARRAY_BUFFER, ebos[accessor.bufferView]);

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
            if(attribute > 0){
                glEnableVertexAttribArray(attribute);
                glVertexAttribPointer(attribute, size, accessor.componentType, accessor.normalized? GL_TRUE : GL_FALSE, byteStride, (char*)NULL + accessor.byteOffset);
            }
        }
    }
}
void GLTFStaticMesh::bindModelNodes(std::map<int, unsigned int>&ebos, tinygltf::Node& node) {
    if((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        bindMesh(ebos, model.meshes[node.mesh]);
    }

    for(size_t i = 0; i < node.children.size(); ++i) {
        assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
        bindModelNodes(ebos, model.nodes[node.children[i]]);
    }
}

float GLTFStaticMesh::prepareForDrawing() {
    glBindVertexArray(VAO_and_EBOs.first);
    return scale;
}

void GLTFStaticMesh::draw() {
    std::cout << "Drawing GLTF model with " << model.meshes.size() << " meshes." << std::endl;
    const tinygltf::Scene & scene = model.scenes[model.defaultScene];
    for(size_t i = 0; i < scene.nodes.size(); ++i) {
        drawModelNodes(model.nodes[scene.nodes[i]]);
    }
}

void GLTFStaticMesh::drawModelNodes(tinygltf::Node & node) {
    if((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        drawMesh(VAO_and_EBOs.second, model.meshes[node.mesh]);
    }

    for(size_t i = 0; i < node.children.size(); ++i) {
        drawModelNodes(model.nodes[node.children[i]]);
    }
}

void GLTFStaticMesh::drawMesh(const std::map<int, unsigned int> & ebos, tinygltf::Mesh & mesh) {
    for(size_t i = 0; i < mesh.primitives.size(); ++i) {
        tinygltf::Primitive & primitive = mesh.primitives[i];
        tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebos.at(indexAccessor.bufferView));
        glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, (char*)NULL + indexAccessor.byteOffset);
    }
}
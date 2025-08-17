#include "GLTFModel.h"
#include "GLTFMesh.h"
#include "Global.h"

namespace MyCraft {
    GLTFModel::GLTFModel(const std::string& modelPath){
        if (modelPath.size()) load(modelPath);
    }

    GLTFModel::~GLTFModel() {
    }

    void GLTFModel::draw() const {
        ((GLTFStaticMesh*)getCore())->Draw();
    }

    GLTFStaticMesh::SetNode& GLTFModel::States() {
        return ((GLTFStaticMesh*)getCore())->States();
    }
    GLTFAnimation& GLTFModel::Animations(const std::string& name) {
        return ((GLTFStaticMesh*)getCore())->Animations(name);
    }

    MyBase::FlyWeightCore* GLTFModel::create(const std::string& src) const {

        tinygltf::TinyGLTF loader;
        std::string error, warning;
        tinygltf::Model model;
        loader.LoadASCIIFromFile(&model, &error, & warning, src);

        if(!warning.empty())std::cout<<"Warning: "<< warning << std::endl;
        if(!error.empty()) throw std::runtime_error("Error: " + error);


        GLTFStaticMesh* mesh = new GLTFStaticMesh(model);
        return mesh;
    }
}
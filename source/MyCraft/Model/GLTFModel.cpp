#include "GLTFModel.h"
#include "Color.h"
#include "GLTFMesh.h"
#include "Global.h"
#include "ShapeManager.h"

namespace MyCraft {
    GLTFModel::GLTFModel(const std::string& modelPath): __baseColor(TRANSPARENCY) {
        if (modelPath.size()) load(modelPath);
        __states = new GLTFStaticMesh::SetNode(((GLTFStaticMesh*)getCore())->getNodeSize());
        __baseColorBuffer = MyBase::ShapeManager::getInstance().createColor(TRANSPARENCY);
    }

    GLTFModel::~GLTFModel() {
        delete __states;
        MyBase::ShapeManager::getInstance().removeColor(__baseColor);
    }

    void GLTFModel::setBaseColor(const MyBase::Color& color) {
        if (color != __baseColor) {
            MyBase::ShapeManager::getInstance().removeColor(__baseColor);
            __baseColor = color;
            __baseColorBuffer = MyBase::ShapeManager::getInstance().createColor(__baseColor);
        }
    }

    void GLTFModel::draw() const {
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, __baseColorBuffer);
        ((GLTFStaticMesh*)getCore())->Draw(*__states);
    }

    GLTFStaticMesh::SetNode& GLTFModel::States() {
        return *__states;;
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
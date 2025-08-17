#include "GLTFModel.h"
#include "GLTFMesh.h"
#include "Global.h"

namespace MyCraft {
    GLTFModel::GLTFModel(const std::string& modelPath):
        m_position(0.0f, 0.0f, 0.0f),
        m_dimensions(1.0f, 1.0f, 1.0f),
        m_rotationAngle(0.0f),
        m_rotationAxis(0.0f, 1.0f, 0.0f)
    {
        if (modelPath.size()) load(modelPath);
    }

    GLTFModel::~GLTFModel() {
    }

    void GLTFModel::Draw(const glm::mat4& modelMatrix) const {
        ((GLTFStaticMesh*)getCore())->Draw();
    }

    glm::mat4x3 GLTFModel::getShape() {
        glm::mat4x3 shape;
        
        shape[0] = m_position - (m_dimensions * m_scale) * 0.5f;
        
        shape[1] = glm::vec3(m_dimensions.x * m_scale, 0.0f, 0.0f); // X-axis (width)
        shape[2] = glm::vec3(0.0f, m_dimensions.y * m_scale, 0.0f); // Y-axis (height)
        shape[3] = glm::vec3(0.0f, 0.0f, m_dimensions.z * m_scale); // Z-axis (depth)
        
        return shape;
    }

    void GLTFModel::setPosition(const glm::vec3& position) { 
        m_position = position; 
    }

    void GLTFModel::move(const glm::vec3& offset) {
        m_position += offset;
    }

    void GLTFModel::setDimensions(const glm::vec3& dimensions) { 
        m_dimensions = dimensions; 
    }

    void GLTFModel::setScale(float scale) { 
        m_scale = scale; 
    }

    void GLTFModel::scale(float factor) {
        m_scale *= factor;
    }

    void GLTFModel::setRotation(float angle, const glm::vec3& axis) {
        m_rotationAngle = angle;
        m_rotationAxis = glm::normalize(axis);
    }

    void GLTFModel::rotate(float angle, const glm::vec3& axis) {
        glm::quat newRotation = glm::angleAxis(angle, glm::normalize(axis));
        glm::quat currentRotation = glm::angleAxis(m_rotationAngle, m_rotationAxis);
        glm::quat finalRotation = newRotation * currentRotation;
        
        m_rotationAngle = glm::angle(finalRotation);
        m_rotationAxis = glm::axis(finalRotation);
    }

    glm::vec3 GLTFModel::getPosition() const { 
        return m_position; 
    }

    glm::vec3 GLTFModel::getDimensions() const { 
        return m_dimensions; 
    }

    float GLTFModel::getScale() const { 
        return m_scale; 
    }

    float GLTFModel::getRotationAngle() const {
        return m_rotationAngle;
    }

    glm::vec3 GLTFModel::getRotationAxis() const {
        return m_rotationAxis;
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
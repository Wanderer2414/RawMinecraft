#pragma once

#include "MyCraft/ModelTest/gltf_mesh.h"

#include <glm/glm.hpp>
#include <string>

class GLTFModel {
public:
    GLTFModel(const std::string& modelPath, float scale = 1.0f);
    ~GLTFModel();

    void draw(const glm::mat4& modelMatrix, 
             const glm::mat4& viewMatrix, 
             const glm::mat4& projectionMatrix);

    glm::mat4x3 getShape();

    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& offset);
    void setDimensions(const glm::vec3& dimensions);
    void setScale(float scale);
    void scale(float factor);

  
    // Lấy thông tin
    glm::vec3 getPosition() const;
    glm::vec3 getDimensions() const;
    float getScale() const;
    float getRotationAngle() const;
    glm::vec3 getRotationAxis() const;

private:
    GLTFStaticMesh m_mesh;       
    glm::vec3 m_position;        
    glm::vec3 m_dimensions;      
    float m_scale;              
    float m_rotationAngle;       
    glm::vec3 m_rotationAxis;   
};
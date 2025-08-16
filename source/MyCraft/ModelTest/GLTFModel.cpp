#include "GLTFModel.h"
#include "Global.h"
#include "UI/ShaderStorage.h"
#include "UI/Controller3D.h"
#include "config.h"
#include <iostream>

GLTFModel::GLTFModel(const std::string& modelPath, float scale) 
    : m_mesh(modelPath.c_str(), scale),
      m_position(0.0f, 0.0f, 0.0f),
      m_dimensions(1.0f, 1.0f, 1.0f),
      m_scale(scale),
      m_rotationAngle(0.0f),
      m_rotationAxis(0.0f, 1.0f, 0.0f)
{
}

GLTFModel::~GLTFModel() {
}

void GLTFModel::draw(const glm::mat4& modelMatrix, 
                    const glm::mat4& viewMatrix, 
                    const glm::mat4& projectionMatrix) {
    glm::mat4 finalModelMatrix = modelMatrix;
    
    finalModelMatrix = glm::translate(finalModelMatrix, m_position);
    finalModelMatrix = glm::rotate(finalModelMatrix, m_rotationAngle, m_rotationAxis);
    finalModelMatrix = glm::scale(finalModelMatrix, glm::vec3(m_scale));


    // PLEASE USING AND EDITTING MODEL SHADER (DONT USE DEFAULT SHADER)
    GLuint shaderProgram = MyBase3D::ShaderStorage::getInstance().GetModelShader();
    glUseProgram(shaderProgram);

    // OLD VERSION OPENGL??? => MODERN OPENGL/OPENGL 4.6
    // GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    // GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    // GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

    //Create uniform buffer

    // SEE INPUT MATRIX AND ATTRIBUTE IN model.vert AND model.frag
    // OVERWRITE AND COMPILE BY glslangValidator -V model.vert -o model.vert.spv OR glslangValidator -V model.frag -o model.frag.spv
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glm::mat4 clipPlane = projectionMatrix*viewMatrix;
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &clipPlane, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, buffer);

    GLuint finalPos;
    glGenBuffers(1, &finalPos);
    glBindBuffer(GL_UNIFORM_BUFFER, finalPos);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), &finalModelMatrix, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, finalPos);

    // if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(finalModelMatrix));
    // if (viewLoc != -1) glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    // if (projLoc != -1) glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    m_mesh.prepareForDrawing();
    m_mesh.draw();
   
    //REMEMBER TO DELETE BUFFER => CHANGE TO CLASS ATTRIBUTE => BUFFER RELATED TO CAMERA IS CAPSULATED IN CAMERA CLASS
    glDeleteBuffers(1, &buffer);
    glDeleteBuffers(1, &finalPos);
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
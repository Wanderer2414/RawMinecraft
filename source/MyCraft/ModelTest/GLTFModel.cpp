#include "GLTFModel.h"
#include "Global.h"
#include "UI/ShaderStorage.h"
#include "UI/Controller3D.h"
#include "config.h"
#include <iostream>

GLTFModel::GLTFModel(const std::string& modelPath, float scale) 
    : m_mesh(modelPath.c_str(), scale),
      m_position(0.0f, 0.0f, 0.0f),
      m_dimensions(1.0f, 1.0f, 1.0f), // Kích thước mặc định 1x1x1
      m_scale(scale),
      m_rotationAngle(0.0f),
      m_rotationAxis(0.0f, 1.0f, 0.0f) // Mặc định xoay quanh trục Y
{
}

GLTFModel::~GLTFModel() {
    // Tự động giải phóng tài nguyên qua destructor của GLTFStaticMesh
}

void GLTFModel::draw(const glm::mat4& modelMatrix, 
                    const glm::mat4& viewMatrix, 
                    const glm::mat4& projectionMatrix) {
    std::cout << "Drawing GLTFModel..." << std::endl;
    // Tạo ma trận model kết hợp vị trí, tỉ lệ và xoay
    glm::mat4 finalModelMatrix = modelMatrix;
    
    // Áp dụng phép biến đổi
    finalModelMatrix = glm::translate(finalModelMatrix, m_position);
    finalModelMatrix = glm::rotate(finalModelMatrix, m_rotationAngle, m_rotationAxis);
    finalModelMatrix = glm::scale(finalModelMatrix, glm::vec3(m_scale));

    std::cout<<"Shape of GLTFModel: "<<std::endl;
    // Kích hoạt shader và truyền uniform

    // PLEASE USING AND EDITTING MODEL SHADER (DONT USE DEFAULT SHADER)
    GLuint shaderProgram = MyBase3D::ShaderStorage::getInstance().getModelShader();
    glUseProgram(shaderProgram);
    std::cout << "Using shader program: " << std::endl;

    // OLD VERSION OPENGL??? => MODERN OPENGL/OPENGL 4.6
    // Truyền các ma trận vào shader
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

    std::cout << "preparing for drawing..." << std::endl;
    // Vẽ model
    m_mesh.prepareForDrawing();
    std::cout << "Drawing mesh..." << std::endl;
    m_mesh.draw();
    std::cout << "GLTFModel drawn successfully!" << std::endl;
   
    //REMEMBER TO DELETE BUFFER => CHANGE TO CLASS ATTRIBUTE => BUFFER RELATED TO CAMERA IS CAPSULATED IN CAMERA CLASS
    glDeleteBuffers(1, &buffer);
    glDeleteBuffers(1, &finalPos);
}

glm::mat4x3 GLTFModel::getShape() {
    glm::mat4x3 shape;
    std::cout<<"GLTFModel::getShape() called."<<std::endl;
    std::cout<<"Position: "<<m_position.x<<", "<<m_position.y<<", "<<m_position.z<<std::endl;
    
    // Góc dưới cùng phía trước bên trái (origin)
    shape[0] = m_position - (m_dimensions * m_scale) * 0.5f;
    
    // Vector kích thước theo các trục
    shape[1] = glm::vec3(m_dimensions.x * m_scale, 0.0f, 0.0f); // X-axis (width)
    shape[2] = glm::vec3(0.0f, m_dimensions.y * m_scale, 0.0f); // Y-axis (height)
    shape[3] = glm::vec3(0.0f, 0.0f, m_dimensions.z * m_scale); // Z-axis (depth)
    
    return shape;
}

// Các hàm di chuyển và biến đổi
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
    // Kết hợp phép xoay mới với phép xoay hiện tại
    glm::quat newRotation = glm::angleAxis(angle, glm::normalize(axis));
    glm::quat currentRotation = glm::angleAxis(m_rotationAngle, m_rotationAxis);
    glm::quat finalRotation = newRotation * currentRotation;
    
    m_rotationAngle = glm::angle(finalRotation);
    m_rotationAxis = glm::axis(finalRotation);
}

// Các hàm getter
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
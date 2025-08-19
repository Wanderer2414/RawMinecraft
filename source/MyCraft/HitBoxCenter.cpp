#include <string>
#include "HitBoxCenter.h"
#include "Container3D.h"
#include "General.h"
#include "Global.h"
#include "Message.h"
#include "ModelController.h"
#include "Pig/ModelController.h"
#include "ShaderStorage.h"

namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {
        __colors = glm::vec3(1,0,0);
        insert(new Pig::Controller());
    }
    HitBoxCenter::~HitBoxCenter() {
        for (int i = 0; i<__models.size(); i++) delete __models[i];
    }

    bool HitBoxCenter::isBusyBlock(const glm::ivec3& position) const {
        return false;
    }
    bool HitBoxCenter::isColistion(const glm::vec3& position) const {
        return false;
    }
    static glm::vec3 position = {0,0,0};
    static float angle = 0;
    bool HitBoxCenter::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Container3D::handle(window);
        {
            angle+=0.002;
            __models[0]->move(glm::vec3(2*cos(angle), 2*sin(angle),0)-position);
            position = glm::vec3(2*cos(angle), 2*sin(angle),0);
        }
        return true;
    }

    void HitBoxCenter::insert(ModelController* model) {
        __models.push_back(model);
        MyBase::Network::match(model);
        Container3D::insert(model);
    }
    void HitBoxCenter::erase(ModelController* model) {
    }
    void HitBoxCenter::glDraw() const {
        MyBase3D::Container3D::glDraw();
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetDefaultShader());
        for (auto& model: __models) {
            glm::mat4x3 mat = model->getShape();
            DrawMargin(mat, __colors);
        }

        

        

        // // Lấy shader program từ ShaderStorage
        // GLuint shaderProgram = MyBase3D::ShaderStorage::getInstance().GetDefaultShader();
        // glUseProgram(shaderProgram);

        // // ===== THÊM PHẦN DEBUG TẠI ĐÂY ===== //
        // // Thiết lập ma trận model/view/projection 
        // glm::mat4 modelMatrix = glm::mat4(1.0f);
        // modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        // modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));

        // // 1. Debug camera - đảm bảo camera nhìn thấy model
        // glm::mat4 view = glm::lookAt(
        //     glm::vec3(0.0f, 0.0f, 3.0f),  // Camera ở (0,0,3)
        //     glm::vec3(0.0f, 0.0f, 0.0f),   // Nhìn vào gốc tọa độ
        //     glm::vec3(0.0f, 1.0f, 0.0f)    // Hướng lên trên
        // );

        // // 2. Debug projection - đảm bảo không cắt model
        // float aspectRatio = 16.0f/9.0f;  // Thay bằng tỉ lệ màn hình thực tế
        // glm::mat4 projection = glm::perspective(
        //     glm::radians(45.0f),         // Góc nhìn 45 độ
        //     aspectRatio, 
        //     0.1f,                        // Near plane
        //     100.0f                       // Far plane
        // );

        // // 3. In giá trị ma trận ra console
        // std::cout << "\n=== DEBUG MATRICES ===" << std::endl;
        // // std::cout << "Model Matrix:\n" << glm::to_string(modelMatrix) << std::endl;
        // // std::cout << "View Matrix:\n" << glm::to_string(view) << std::endl;
        // // std::cout << "Projection Matrix:\n" << glm::to_string(projection) << std::endl;

        // // 4. Truyền ma trận vào shader
        // GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        // GLuint viewLoc = glGetUniformLocation(shaderProgram, "view"); 
        // GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

        // if (modelLoc == -1 || viewLoc == -1 || projLoc == -1) {
        //     std::cerr << "ERROR: Missing required uniforms!" << std::endl;
        // }

        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // // ===== KẾT THÚC PHẦN DEBUG ===== //

        // // Vẽ model GLTF
        // float scale = modelTest->prepareForDrawing();
        // modelTest->draw();
    }
}
#include "HitBoxCenter.h"
#include "Global.h"
#include "Model.h"
#include "PointSet.h"
#include "ShaderStorage.h"
namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {

    }
    HitBoxCenter::~HitBoxCenter() {

    }
        
    void HitBoxCenter::insert(Model* model) {
        __models.push_back(model);
    }
    void HitBoxCenter::erase(Model* model) {
    }
    void HitBoxCenter::glDraw() const {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetDefaultShader());
        for (auto& model: __models) {
            glm::mat4x3 bottom[2] = {model->getBottomRec()};
            bottom[1] = bottom[0];
            for (int i = 0; i<4; i++) bottom[1][i].z += model->getZRange().y;

            GLuint VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 96, &bottom, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());

            // glDrawArrays(GL_LINE_STRIP, 0, 8);
            glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }
    }
}
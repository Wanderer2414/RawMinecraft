#include "HitBoxCenter.h"
#include "Global.h"
#include "ModelController.h"
#include "PointSet.h"
#include "ShaderStorage.h"
namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {
        float colors[24];
        for (int i = 0; i<8; i++) {
            colors[3*i] = 1;
            colors[3*i+1] = 0;
            colors[3*i+2] = 0;
        }
        glGenBuffers(1, &__colors);
        glBindBuffer(GL_ARRAY_BUFFER, __colors);
        glBufferData(GL_ARRAY_BUFFER, 96, colors, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    HitBoxCenter::~HitBoxCenter() {
        glDeleteBuffers(1, &__colors);
    }
    void HitBoxCenter::insert(ModelController* model) {
        __models.push_back(model);
    }
    void HitBoxCenter::erase(ModelController* model) {
    }
    void HitBoxCenter::glDraw() const {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetDefaultShader());
        for (auto& model: __models) {
            glm::mat4x3 mat = model->getShape();
            glm::vec3 shape[8] = {mat[0], mat[0]+mat[1], mat[0] + mat[1]+ mat[2], mat[0] +mat[2]};
            for (int i = 4; i<8; i++) shape[i] = shape[i-4]+mat[3];

            GLuint VAO, VBO;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, 96, &shape, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

            glBindBuffer(GL_ARRAY_BUFFER, __colors);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());

            
            glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }
    }
}
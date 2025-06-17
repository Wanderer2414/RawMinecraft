#include "HitBoxCenter.h"
#include "General.h"
#include "Global.h"
#include "ModelController.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "glm/geometric.hpp"
namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {
        __colors = glm::vec3(1,0,0);
    }
    HitBoxCenter::~HitBoxCenter() {
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
            DrawMargin(mat, __colors);
        }
    }
}
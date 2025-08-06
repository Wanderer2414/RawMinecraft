#include "HitBoxCenter.h"
#include "General.h"
#include "Global.h"
#include "ModelController.h"
#include "ShaderStorage.h"

namespace MyCraft {
    
    HitBoxCenter::HitBoxCenter() {
        __colors = glm::vec3(1,0,0);
    }
    HitBoxCenter::~HitBoxCenter() {
    }

    bool HitBoxCenter::isBusyBlock(const glm::ivec3& position) const {
        return false;
    }
    bool HitBoxCenter::isColistion(const glm::vec3& position) const {
        return false;
    }
    void HitBoxCenter::insert(ModelController* model) {
        __models.push_back(model);
    }
    void HitBoxCenter::erase(ModelController* model) {
    }
    void HitBoxCenter::glDraw() const {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetDefaultShader());
        for (auto& model: __models) {
            glm::mat4x3 mat = model->getShape();
            DrawMargin(mat, __colors);
        }
    }
}
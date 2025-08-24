#include "HitBoxCenter.h"
#include "Container3D.h"
#include "Cow/ModelController.h"
#include "General.h"
#include "Global.h"
#include "Message.h"
#include "ShaderStorage.h"
#include "SkeletonBoss/ModelController.h"
#include "Pig/ModelController.h"
#include "Zombie/ModelController.h"

namespace MyCraft {
    HitBoxCenter::HitBoxCenter() {
        __colors = glm::vec3(1,0,0);
        insert(new Zombie::Controller());
        insert(new SkeletonBoss::ModelController());
    }
    HitBoxCenter::~HitBoxCenter() {
        for (int i = 0; i<__models.size(); i++) delete __models[i];
    }
    bool HitBoxCenter::isHover() const {
        return __hoverEntity;
    }
    bool HitBoxCenter::isBusyBlock(const glm::ivec3& position) const {
        return false;
    }
    bool HitBoxCenter::isColistion(const glm::vec3& position) const {
        return false;
    }

    ModelController* HitBoxCenter::isColistion(const glm::vec3& position, const glm::vec3& direction) const {
        return (ModelController*)__tree.get(position, direction);
    }
    void HitBoxCenter::setHoverEntity(ModelController* controller) {
        __hoverEntity = controller;
    }

    void HitBoxCenter::attackEntity(const unsigned int& damage, const glm::vec3& direction) {
        if (__hoverEntity) {
            glm::vec3 dir = direction;
            dir.z = 0;
            if (glm::length(dir)) dir = glm::normalize(dir)*0.5f;
            dir.z = 0.2;
            __hoverEntity->move(dir);
            __hoverEntity->damage(damage);
        }
    }
    void HitBoxCenter::feedEntity(const unsigned int& health, const glm::vec3& direction) {
        // if (__hoverEntity) ;
    }

    bool HitBoxCenter::handle(GLFWwindow* window) {
        bool is_changed = MyBase3D::Container3D::handle(window);
        return is_changed;
    }

    void HitBoxCenter::pushPlayerModel(ModelController* model) {
        MyBase::Network::match(model);
        Container3D::insert(model);
        __tree.insert(model);
    }

    void HitBoxCenter::insert(ModelController* model) {
        __models.push_back(model);
        MyBase::Network::match(model);
        Container3D::insert(model);
        __tree.insert(model);
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
    }
}
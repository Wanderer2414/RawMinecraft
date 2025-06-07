#include "Model.h"
#include "Global.h"
#include "ModelLoader.h"
#include "ModelStorage.h"

namespace MyCraft {
    Model::Model(): MyBase3D::Controller3D() {
        __clock.setDuration(10);
        __sec = 0;
    }
    Model::~Model() {
    }
    bool Model::handle(GLFWwindow* window) {
        if (__clock.get()) {
            __sec += 0.02;
            return true;
        }
        return false;
    }
    void Model::glDraw() const {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        ModelLoader& model = ModelStorage::Default->getPlayerModel();
        
        std::vector<glm::mat4> tmp = model.get("walk", __sec);
        ModelStorage::Default->DrawModel(tmp, model);
        glDeleteVertexArrays(1, &VAO);
    }
}
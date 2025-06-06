#include "Model.h"
#include "Global.h"
#include "ModelStorage.h"
#include "PointSet.h"
#include "ShaderStorage.h"

namespace MyCraft {
    Model::Model(): MyBase3D::Controller3D() {
    }
    Model::~Model() {
    }

    void Model::glDraw() const {
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        ModelStorage::Default->DrawModel(ModelStorage::Default->getPlayerModel());
        glDeleteVertexArrays(1, &VAO);
    }
}
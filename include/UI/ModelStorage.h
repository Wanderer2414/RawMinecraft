#ifndef MODEL_STORAGE_H
#define MODEL_STORAGE_H
#include "Global.h"
namespace MyCraft {
    class ModelStorage {
    public:
        ModelStorage();
        ~ModelStorage();
        static ModelStorage* Default;
        void DrawModel();
    private:
        tinygltf::Model                         __playerModel;
        std::vector<GLuint>                     __playerModelPoint;
        GLuint                                  __nodeState;
        void    __loadPlayerModel(), 
                __freePlayerModel(), 
                __drawNode(const int& node, glm::mat4 offset, tinygltf::Model& model),
                __drawMesh(const int& mesh, std::vector<GLuint>& set, tinygltf::Model& model);
    };
}
#endif
#ifndef MODEL_STORAGE_H
#define MODEL_STORAGE_H
#include "Global.h"
#include "ModelLoader.h"

namespace MyCraft {
    class ModelStorage {
    public:
        ModelStorage();
        ~ModelStorage();
        static ModelStorage* Default;
        void DrawModel(std::vector<glm::mat4>& state, const ModelLoader& model);
        ModelLoader                             &getPlayerModel();
    private:
        ModelLoader                             __playerModel;
        
        GLuint                                  __nodeState;
        void    __drawNode(const int& node, std::vector<glm::mat4>& states, const ModelLoader& model),
                __drawMesh(const int& mesh, const ModelLoader& model);

    };
}
#endif
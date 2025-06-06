#ifndef MODEL_STORAGE_H
#define MODEL_STORAGE_H
#include "Global.h"
#include <queue>
namespace MyCraft {
    class ModelStorage {
    public:
        ModelStorage();
        ~ModelStorage();
        static ModelStorage* Default;
        void DrawModel(const tinygltf::Model& model);
        const tinygltf::Model&                        getPlayerModel() const;
    private:
        tinygltf::Model                         __playerModel;
        std::map<const tinygltf::Model*,std::vector<GLuint>> __modelPoint;
        
        GLuint                                  __nodeState;
        void    __loadPlayerModel(), 
                __freePlayerModel(), 
                __drawNode(const int& node, glm::mat4 offset, const tinygltf::Model& model),
                __drawMesh(const int& mesh, const std::vector<GLuint>& set, const tinygltf::Model& model);

    };
}
#endif
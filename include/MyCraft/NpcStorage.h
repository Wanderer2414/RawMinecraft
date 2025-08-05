#ifndef NPC_STORAGE_H
#define NPC_STORAGE_H
#include <vector>
#include <string>
#include "Global.h"
#include "ModelLoader.h"
#include "NpcController.h"
#include "PlayerModelController.h"

namespace MyCraft {
    class NPCStorage {
    public:
        NPCStorage(std::string & source);
        ~NPCStorage();
        void DrawModel(std::vector<glm::mat4>& state, const ModelLoader& model);
        ModelLoader                             &getNPCModel();

    private:
        ModelLoader                             __NPCModel;
        
        GLuint                                  __nodeState;
        void    __drawNode(const int& node, std::vector<glm::mat4>& states, const ModelLoader& model),
                __drawMesh(const int& mesh, const ModelLoader& model);

    };
}
#endif
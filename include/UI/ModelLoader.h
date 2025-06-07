#ifndef MODELLOADER_H
#define MODELLOADER_H
#include "Global.h"
namespace MyCraft {
    class ModelStorage;
    class ModelLoader {
    public:
        class Animation {
        public:
            Animation(const tinygltf::Animation& animation, const ModelLoader& model);
            ~Animation();

            std::vector<glm::mat4> operator[](float x);
            float       getLength() const;

        private:
            float                                                   __length;
            std::vector<std::queue<std::pair<float, glm::quat>>>    __animation;
        };
        ModelLoader(const std::string& source);
        ~ModelLoader();

        int getNodeCount() const;

        friend MyCraft::ModelStorage;
        std::vector<glm::mat4> get(const std::string& name, const float& t);
    private:
        tinygltf::Model     __model;
        std::vector<GLuint> __buffers;
        std::map<std::string, Animation*> __animations;
    };
}
#endif
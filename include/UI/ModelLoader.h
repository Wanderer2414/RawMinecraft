#ifndef MODELLOADER_H
#define MODELLOADER_H
#include "Global.h"
namespace MyCraft {
    class ModelStorage;
    class ModelLoader {
    public:
        class Animation {
        public:
            Animation(tinygltf::Animation& animation,tinygltf::Model& model);
            ~Animation();

            void apply(std::vector<glm::mat4>&, float x, tinygltf::Model& model);
            float       getLength() const;

        private:
            float                                                   __length;
            tinygltf::Animation&                                    __animation;
            std::vector<int>                                        __pointers;
        };
        ModelLoader(const std::string& source);
        ~ModelLoader();

        int getNodeCount() const;
        int getHead() const;
        bool apply(std::vector<glm::mat4>&, const std::string& name, const float& t);
        friend MyCraft::ModelStorage;
    private:
        int     __head;
        tinygltf::Model     __model;
        std::vector<GLuint> __buffers;
        std::map<std::string, Animation*> __animations;
    };
}
#endif
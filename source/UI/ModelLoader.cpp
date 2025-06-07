#include "ModelLoader.h"
#include "Global.h"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include <vector>
namespace  MyCraft {

    ModelLoader::Animation::Animation(const tinygltf::Animation& animation, const ModelLoader& model): __length(0) {
        __animation.resize(model.getNodeCount());
        for (const auto& channel: animation.channels) {
            const tinygltf::AnimationSampler& sampler = animation.samplers[channel.sampler];
            if (channel.target_path == "rotation") {
                const tinygltf::Accessor& TimeAccessor = model.__model.accessors[sampler.input];
                const tinygltf::Accessor& ValueAccessor = model.__model.accessors[sampler.output];
                const tinygltf::BufferView& TimeView = model.__model.bufferViews[TimeAccessor.bufferView];
                const tinygltf::BufferView& ValueView = model.__model.bufferViews[ValueAccessor.bufferView];
                float* time = (float*)(model.__model.buffers[TimeView.buffer].data.data() + TimeView.byteOffset + TimeAccessor.byteOffset);
                glm::quat* value = (glm::quat*)(model.__model.buffers[ValueView.buffer].data.data() + ValueView.byteOffset + ValueAccessor.byteOffset);
                for (int i = 0; i<TimeAccessor.count; i++) {;
                    for (int child:model.__model.nodes[channel.target_node].children)
                        __animation[child].push({time[i], value[i]});
                    __length = std::max(__length, time[i]);
                }
            }
        }
    }
    ModelLoader::Animation::~Animation() {}

    std::vector<glm::mat4> ModelLoader::Animation::operator[](float t) {
        std::vector<glm::mat4> ans(__animation.size(), glm::mat4(1));
        for (int i = 0; i<ans.size(); i++) {
            auto& animation = __animation[i];
            if (animation.size()) {
                while (animation.front().first <= t) {
                    animation.push(animation.front());
                    animation.pop();
                    if (animation.front().first<animation.back().first) animation.front().first+=__length;
                }
                float ratio;
                if (animation.front().first<animation.back().first) 
                    ratio = (t-animation.back().first)/(animation.front().first-animation.back().first+__length);
                else ratio = (t-animation.back().first)/(animation.front().first-animation.back().first);
                ans[i] = glm::toMat4(glm::normalize(glm::slerp(animation.back().second, animation.front().second, ratio)));
            }
        }
        return ans;
    }

    float ModelLoader::Animation::getLength() const {
        return __length;
    }

    ModelLoader::ModelLoader(const std::string& source) {

        tinygltf::TinyGLTF loader;
        std::string err, warn;
        if (!loader.LoadASCIIFromFile(&__model, &err, &warn, source)) {
            std::cout << "Failed to load player model" << std::endl;
            exit(0);
        }
        const tinygltf::Scene& scenes = __model.scenes[__model.defaultScene];
        __buffers.resize(__model.bufferViews.size());
        for (std::size_t i = 0; i<__model.bufferViews.size(); i++) {
            tinygltf::BufferView& bufferView = __model.bufferViews[i];
            tinygltf::Buffer& buffer = __model.buffers[bufferView.buffer];
            float* data = (float*)(buffer.data.data()  + bufferView.byteOffset);
    
            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(bufferView.target, VBO);
            glBufferData(bufferView.target, bufferView.byteLength, data, GL_STATIC_DRAW);
            glBindBuffer(bufferView.target, 0);
            __buffers[i] = VBO;
        }
        for (const auto& animation: __model.animations) {
            __animations[animation.name] = new ModelLoader::Animation(animation, *this);
        }
    }
    ModelLoader::~ModelLoader() {
        for (int i = 0; i<__buffers.size(); i++)
            glDeleteBuffers(1, &__buffers[i]);
        for (auto& i:__animations) delete i.second;
    }   
    std::vector<glm::mat4> ModelLoader::get(const std::string& name, const float& t) {
        return __animations[name]->operator[](t);
    }
    int ModelLoader::getNodeCount() const {
        return __model.nodes.size();
    }
}
#include "ModelLoader.h"
#include "Global.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include <vector>
namespace  MyCraft {

    ModelLoader::Animation::Animation(tinygltf::Animation& animation, tinygltf::Model& model): __length(0), __animation(animation) {
        __pointers.resize(animation.channels.size(), 0);
    }
    ModelLoader::Animation::~Animation() {}
    void ModelLoader::Animation::apply(std::vector<glm::mat4>& out, float t, tinygltf::Model& model) {
        if (t>=__length) {
            int n = t/__length;
            t -= n*__length;
        }
        if (out.size()!=model.nodes.size()) 
            out.resize(model.nodes.size(), glm::mat4(1));
        for (int i  = 0; i<__animation.channels.size(); i++) {
            const auto& channel = __animation.channels[i];
            const tinygltf::AnimationSampler& sampler = __animation.samplers[channel.sampler];
            if (channel.target_path == "rotation") {
                const tinygltf::Accessor& TimeAccessor = model.accessors[sampler.input];
                const tinygltf::Accessor& ValueAccessor = model.accessors[sampler.output];
                const tinygltf::BufferView& TimeView = model.bufferViews[TimeAccessor.bufferView];
                const tinygltf::BufferView& ValueView = model.bufferViews[ValueAccessor.bufferView];
                float* time = (float*)(model.buffers[TimeView.buffer].data.data() + TimeView.byteOffset + TimeAccessor.byteOffset);
                glm::quat* value = (glm::quat*)(model.buffers[ValueView.buffer].data.data() + ValueView.byteOffset + ValueAccessor.byteOffset);
                auto& pointer = __pointers[i];
                glm::mat4 tmp;
                if (t<time[pointer]) pointer = 0;
                while (pointer+1<TimeAccessor.count && t>=time[pointer+1]) pointer++;
                float ratio;
                if (pointer+1<TimeAccessor.count) {
                    ratio = (t-time[pointer])/(time[pointer+1]-time[pointer]);
                    tmp = glm::toMat4(glm::normalize(glm::slerp(value[pointer], value[pointer+1], ratio)));
                }
                else tmp = glm::toMat4(glm::normalize(value[pointer]));
                for (int child: model.nodes[channel.target_node].children) 
                    out[child] *= tmp;
            }
            else if (channel.target_path == "translation") {
                const tinygltf::Accessor& TimeAccessor = model.accessors[sampler.input];
                const tinygltf::Accessor& ValueAccessor = model.accessors[sampler.output];
                const tinygltf::BufferView& TimeView = model.bufferViews[TimeAccessor.bufferView];
                const tinygltf::BufferView& ValueView = model.bufferViews[ValueAccessor.bufferView];
                float* time = (float*)(model.buffers[TimeView.buffer].data.data() + TimeView.byteOffset + TimeAccessor.byteOffset);
                float* value = (float*)(model.buffers[ValueView.buffer].data.data() + ValueView.byteOffset + ValueAccessor.byteOffset);
                auto& pointer = __pointers[i];
                glm::mat4 tmp;
                if (t<=time[pointer]) pointer = 0;
                while (pointer+1<TimeAccessor.count && t>=time[pointer+1]) pointer++;
                if (pointer+1 < TimeAccessor.count) {
                    float ratio = (t-time[pointer])/(time[pointer+1]-time[pointer]);
                    // out[channel.target_node] = glm::translate(glm::mat4(1), value[pointer]*ratio + value[pointer+1]*(1-ratio));
                }
                else {
                    glm::vec3 tmp(model.nodes[channel.target_node].translation[0], model.nodes[channel.target_node].translation[2], model.nodes[channel.target_node].translation[1]);
                    tmp.x =tmp.x - value[pointer*3] ;
                    tmp.y = value[pointer*3+2] - tmp.y;
                    tmp.z = tmp.z - value[pointer*3+1];
                    for (int child:model.nodes[channel.target_node].children) {
                        out[child]= glm::translate(out[child], -tmp);
                    }
                }

            }
        }
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
        for (auto& animation: __model.animations) {
            __animations[animation.name] = new ModelLoader::Animation(animation, __model);
        }
        for (int i = 0; i<__model.nodes.size(); i++) {
            if (__model.nodes[i].name == "head") __head = i;
        }
    }
    ModelLoader::~ModelLoader() {
        for (int i = 0; i<__buffers.size(); i++)
            glDeleteBuffers(1, &__buffers[i]);
        for (auto& i:__animations) delete i.second;
    }   
    int ModelLoader::getHead() const {
        return __head;
    }
    bool ModelLoader::apply(std::vector<glm::mat4>& out, const std::string& name, const float& t) {
        if (__animations.find(name)==__animations.end()) return false;
        __animations[name]->apply(out, t, __model);
        return true;
    }
    int ModelLoader::getNodeCount() const {
        return __model.nodes.size();
    }
}
#include "ShaderStorage.h"
#include "Global.h"
#include "spriv_extended.h"
namespace MyBase3D {
    ShaderStorage* ShaderStorage::Default;
    ShaderStorage::ShaderStorage() {
        __defaultShader = createProgram("assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
        __cubeShader = createProgram("assets/shaders/cube.vert.spv", "assets/shaders/cube.frag.spv");
        __marginShader = createProgram("assets/shaders/margin.vert.spv", "assets/shaders/shader.frag.spv");
        __point2DShader = createProgram("assets/shaders/Point2D.vert.spv", "assets/shaders/Point2D.frag.spv");
        __modelShader = createProgram("assets/shaders/model.vert.spv", "assets/shaders/shader.frag.spv");
        __fontShader = createProgram("assets/shaders/font2D.vert.spv", "assets/shaders/font2D.frag.spv");
        __imageShader = createProgram("assets/shaders/image2D.vert.spv", "assets/shaders/image2D.frag.spv");
    }
    ShaderStorage::~ShaderStorage() {
        glDeleteProgram(__defaultShader);
        glDeleteProgram(__cubeShader);
        glDeleteProgram(__marginShader);
        glDeleteProgram(__point2DShader);
        glDeleteProgram(__modelShader);
        glDeleteProgram(__fontShader);
        glDeleteProgram(__imageShader);
    }

    ShaderStorage& ShaderStorage::getInstance() {
        if (!ShaderStorage::Default) ShaderStorage::Default = new ShaderStorage();
        return *ShaderStorage::Default;
    }
    void ShaderStorage::close() {
        if (ShaderStorage::Default) {
            delete ShaderStorage::Default;
            ShaderStorage::Default = 0;
        }
    }
    GLuint ShaderStorage::GetDefaultShader() const {
        return __defaultShader;
    }
    GLuint ShaderStorage::GetCubeShader() const {
        return __cubeShader;
    }
    GLuint ShaderStorage::GetMarginShader() const {
        return __marginShader;
    }
    GLuint ShaderStorage::GetPoint2DShader() const {
        return __point2DShader;
    }
    GLuint ShaderStorage::GetModelShader() const {
        return __modelShader;
    }
    GLuint ShaderStorage::GetFontShader() const {
        return __fontShader;
    }
    // GLuint ShaderStorage::GetChunkShader() const {
    //     return __chunkShader;
    // }
    GLuint ShaderStorage::GetImage2DShader() const {
        return __imageShader;
    }
}
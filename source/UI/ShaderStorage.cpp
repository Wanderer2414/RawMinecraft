#include "ShaderStorage.h"
#include "Global.h"
#include "spriv_extended.h"
namespace MyBase3D {

    ShaderStorage::ShaderStorage() {
        __defaultShader = createProgram("assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
        __cubeShader = createProgram("assets/shaders/cube.vert.spv", "assets/shaders/cube.frag.spv");
        __chunkShader = createProgram("assets/shaders/chunk.vert.spv", "assets/shaders/chunk.frag.spv");
        __marginShader = createProgram("assets/shaders/margin.vert.spv", "assets/shaders/shader.frag.spv");
        __point2DShader = createProgram("assets/shaders/Point2D.vert.spv", "assets/shaders/Point2D.frag.spv");
        __modelShader = createProgram("assets/shaders/model.vert.spv", "assets/shaders/shader.frag.spv");
        __image2DShader = createProgram("assets/shaders/Image2D.vert.spv", "assets/shaders/cube.frag.spv");
        __fontShader = createProgram("assets/shaders/font2D.vert.spv", "assets/shaders/font2D.frag.spv");
    }
    ShaderStorage::~ShaderStorage() {
        glDeleteProgram(__defaultShader);
        glDeleteProgram(__cubeShader);
        glDeleteProgram(__chunkShader);
        glDeleteProgram(__marginShader);
        glDeleteProgram(__point2DShader);
        glDeleteProgram(__modelShader);
        glDeleteProgram(__image2DShader);
        glDeleteProgram(__fontShader);
    }
    GLuint ShaderStorage::GetChunkShader() const {
        return __chunkShader;
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
    GLuint ShaderStorage::getPoint2DShader() const {
        return __point2DShader;
    }
    GLuint ShaderStorage::getModelShader() const {
        return __modelShader;
    }
    GLuint ShaderStorage::getImage2DShader() const {
        return __image2DShader;
    }
    GLuint ShaderStorage::getFontShader() const {
        return __fontShader;
    }
}
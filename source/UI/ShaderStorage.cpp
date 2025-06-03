#include "ShaderStorage.h"
#include "spriv_extended.h"
namespace MyBase3D {

    ShaderStorage::ShaderStorage() {
        __defaultShader = createProgram("assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
        __cubeShader = createProgram("assets/shaders/cube.vert.spv", "assets/shaders/cube.frag.spv");
        __chunkShader = createProgram("assets/shaders/chunk.vert.spv", "assets/shaders/chunk.frag.spv");
        __marginShader = createProgram("assets/shaders/margin.vert.spv", "assets/shaders/shader.frag.spv");
        __point2DShader = createProgram("assets/shaders/Point2D.vert.spv", "assets/shaders/Point2D.frag.spv");
    }
    ShaderStorage::~ShaderStorage() {
        glDeleteProgram(__defaultShader);
        glDeleteProgram(__cubeShader);
        glDeleteProgram(__chunkShader);
        glDeleteProgram(__marginShader);
        glDeleteProgram(__point2DShader);
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
}
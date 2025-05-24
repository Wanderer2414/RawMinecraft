#include "ShaderStorage.h"
#include "spriv_extended.h"

ShaderStorage::ShaderStorage() {
    DefaultShader = createProgram("assets/shaders/shader.vert.spv", "assets/shaders/shader.frag.spv");
    CubeShader = createProgram("assets/shaders/cube.vert.spv", "assets/shaders/cube.frag.spv");
}
ShaderStorage::~ShaderStorage() {
    glDeleteProgram(DefaultShader);
    glDeleteProgram(CubeShader);
}
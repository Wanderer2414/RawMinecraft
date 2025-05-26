#ifndef SHADER_STORAGE_H
#define SHADER_STORAGE_H
#include "Global.h"
class ShaderStorage {
public:
    ShaderStorage();
    ~ShaderStorage();

    static ShaderStorage* Default;

    GLuint DefaultShader;
    GLuint CubeShader;
    GLuint ChunkShader;
    GLuint MarginShader;
private:
};
#endif
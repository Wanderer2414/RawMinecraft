#ifndef SHADER_STORAGE_H
#define SHADER_STORAGE_H
#include "Global.h"
namespace MyBase3D {

class ShaderStorage {
    public:
        static ShaderStorage& getInstance();
        static void close();
    
        GLuint  GetDefaultShader() const,
                GetCubeShader() const,
                GetChunkShader() const,
                GetMarginShader() const,
                getPoint2DShader() const,
                getModelShader() const,
                getFontShader() const,
                getImage2DShader() const;
    private:
        ShaderStorage();
        ~ShaderStorage();
        static ShaderStorage* Default;
        GLuint  __defaultShader,
                __cubeShader,
                __point2DShader,
                __marginShader,
                __chunkShader,
                __modelShader,
                __imageShader,
                __fontShader;
    };
}
#endif
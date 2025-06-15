#ifndef SHADER_STORAGE_H
#define SHADER_STORAGE_H
#include "Global.h"
namespace MyBase3D {

class ShaderStorage {
    public:
        ShaderStorage();
        ~ShaderStorage();
    
        static ShaderStorage* Default;
    
        GLuint  GetDefaultShader() const,
                GetCubeShader() const,
                GetChunkShader() const,
                GetMarginShader() const,
                getPoint2DShader() const,
                getModelShader() const,
                getFontShader() const,
                getImage2DShader() const;
    private:
        GLuint  __defaultShader,
                __cubeShader,
                __chunkShader,
                __point2DShader,
                __marginShader,
                __modelShader,
                __fontShader,
                __image2DShader;
    };
}
#endif
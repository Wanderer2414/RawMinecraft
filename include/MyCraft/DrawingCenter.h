#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
#include "Color.h"
#include "Texture.h"
#include <memory>
namespace MyCraft {
    class DrawingCenter {
    public:
        #define SWAP_BUFFER 3
        static void BindCube(const MyBase::Texture& texture, const glm::vec2& extra = {1,1});
        static void BindMargin();
        static void DrawCubes(void* state, const int& size);
        static void DrawMargins(void* data, const int& size, const MyBase::Color& color, const int& lineWidth);
        static void BindSpecialBlock(const MyBase::Texture& texture, const glm::vec2& extra = {1,1});
        static void DrawSpecialBlock(void* data, void* state, const int& size);
        static void Close();
    private:
        static void getInstance();
        ~DrawingCenter();
        DrawingCenter();
        static DrawingCenter* Default;
        unsigned char   __positionBufferPointer;
        GLuint __vertexArray, __positionBuffer[SWAP_BUFFER], __extraBuffer;
    };
};
#endif
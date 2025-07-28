#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
#include "Color.h"
#include <memory>
namespace MyCraft {
    class DrawingCenter {
    public:
        #define SWAP_BUFFER 3
        static void BindCube();
        static void BindMargin();
        static void DrawCubes(void* data, const int& size);
        static void DrawMargins(void* data, const int& size, const MyBase::Color& color, const int& lineWidth);
        static void Close();
    private:
        static void getInstance();
        ~DrawingCenter();
        DrawingCenter();
        static DrawingCenter* Default;
        unsigned char   __positionBufferPointer;
        GLuint __vertexArray, __positionBuffer[SWAP_BUFFER], __texture;
    };
};
#endif
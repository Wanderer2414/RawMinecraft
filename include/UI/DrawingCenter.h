#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
namespace MyCraft {
    class DrawingCenter {
    public:
        DrawingCenter();
        ~DrawingCenter();
        #define SWAP_BUFFER 3
        static DrawingCenter* Default;
        void BindCube();
        void BindChunk();
        void DrawCubes(const MyCraft::BlockCatogary::Catogary& type, void* data, const int& size);
        void DrawChunks(void* data, const int& size);
        void BindMargin();
        void DrawMargin(void* data, const int& size, const int& lineWidth);
    private:
        unsigned char   __positionBufferPointer;
        GLuint __vertexArray, __positionBuffer[SWAP_BUFFER];
    };
};
#endif
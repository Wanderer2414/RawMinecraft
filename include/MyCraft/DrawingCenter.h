#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
namespace MyCraft {
    class DrawingCenter {
    public:
        #define SWAP_BUFFER 3
        static void close();
        static DrawingCenter& getInstance();
        void BindCube();
        void BindChunk();
        void DrawCubes(const MyCraft::BlockCatogary::Catogary& type, void* data, const int& size);
        void DrawChunks(void* data, const int& size);
        void BindMargin();
        void DrawMargin(void* data, const int& size, const int& lineWidth);
    private:
        DrawingCenter();
        ~DrawingCenter();
        static DrawingCenter* Default;
        unsigned char   __positionBufferPointer;
        GLuint __vertexArray, __positionBuffer[SWAP_BUFFER];
    };
};
#endif
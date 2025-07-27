#include "DrawingCenter.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "TextureStorage.h"
namespace MyCraft {
    DrawingCenter* DrawingCenter::Default;
    DrawingCenter::DrawingCenter() {
        __texture = MyBase::TextureStorage::getInstance().getTexture("assets/images/blockCatogary.png");
        glGenVertexArrays(1, &__vertexArray);
        glGenBuffers(SWAP_BUFFER, __positionBuffer);

        for (int i = 0; i<SWAP_BUFFER; i++) {
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[i]);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*4*32, 0, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

    }
    
    DrawingCenter::~DrawingCenter() {
        MyBase::TextureStorage::getInstance().removeTexture("assets/images/blockCatogary.png");
        glDeleteBuffers(SWAP_BUFFER, __positionBuffer);
        glDeleteVertexArrays(1, &__vertexArray);
    }
    DrawingCenter& DrawingCenter::getInstance() {
        if (!Default) Default = new DrawingCenter();
        return *Default;
    }
    void DrawingCenter::close() {
        if (Default) {
            delete Default;
            Default = 0;
        }
    }
    void DrawingCenter::BindCube() {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetCubeShader());
        glBindVertexArray(__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getImageBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::getInstance().getBlockSet());
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, MyBase3D::PointSet::getInstance().getBlockUVS());
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, __texture);
    }
    void DrawingCenter::BindChunk() {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetChunkShader());
    
        glBindVertexArray(__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getMarginBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::getInstance().getBlockSet());
    }
    void DrawingCenter::DrawChunks(void* data, const int& size) {
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __positionBuffer[__positionBufferPointer]);    
            
            for (int j = 0; j<sz*17; j+=17)
                glDrawArrays(GL_LINE_STRIP, j, 17);
            data = (char*)data + sizeof(GLfloat)*4*sz;
            __positionBufferPointer = (__positionBufferPointer+1)%SWAP_BUFFER;
        }
    }
    void DrawingCenter::DrawCubes(void* data, const int& size) {
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __positionBuffer[__positionBufferPointer]);

            glDrawArrays(GL_TRIANGLES, 0, 36*sz);
            data = (char*)data + sizeof(GLfloat)*4*sz;
            __positionBufferPointer = (__positionBufferPointer+1)%SWAP_BUFFER;
        }
    }
    void DrawingCenter::BindMargin() {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetMarginShader());
        glBindVertexArray(__vertexArray);
    
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getMarginBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::getInstance().getBlockSet());
    
    }
    void DrawingCenter::DrawMargin(void* data, const int& size, const int& lineWidth) {
        glLineWidth(lineWidth);
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, __positionBuffer[__positionBufferPointer]);    
            for (int j = 0; j<sz; j++) 
                glDrawArrays(GL_LINE_STRIP, j*17, 17);
            data = (char*)data + sizeof(GLfloat)*4*sz;
            __positionBufferPointer = (__positionBufferPointer+1)%SWAP_BUFFER;
        }

    }
};
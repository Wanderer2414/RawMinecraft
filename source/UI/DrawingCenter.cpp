#include "DrawingCenter.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
namespace MyCraft {

    DrawingCenter::DrawingCenter() {
        glGenVertexArrays(1, &__vertexArray);
        glGenBuffers(SWAP_BUFFER, __positionBuffer);

        for (int i = 0; i<SWAP_BUFFER; i++) {
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[i]);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*4*32, 0, GL_DYNAMIC_DRAW);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    DrawingCenter::~DrawingCenter() {
        glDeleteBuffers(SWAP_BUFFER, __positionBuffer);
        glDeleteVertexArrays(1, &__vertexArray);
    }
    void DrawingCenter::BindCube() {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetCubeShader());
        glBindVertexArray(__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getImageBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::Default->getBlockSet());
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, MyCraft::BlockCatogary::Default->getTexCoord());
    
        glActiveTexture(GL_TEXTURE0);
    }
    void DrawingCenter::BindChunk() {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetChunkShader());
    
        glBindVertexArray(__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::Default->getBlockSet());
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
    void DrawingCenter::DrawCubes(const MyCraft::BlockCatogary::Catogary& type, void* data, const int& size) {    
        glBindTexture(GL_TEXTURE_2D, MyCraft::BlockCatogary::Default->getBlock(type));
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
        glUseProgram(MyBase3D::ShaderStorage::Default->GetMarginShader());
        glBindVertexArray(__vertexArray);
    
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::Default->getBlockSet());
    
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
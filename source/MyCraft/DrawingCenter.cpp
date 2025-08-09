#include "DrawingCenter.h"
#include "Color.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"
#include "Texture.h"
namespace MyCraft {
    DrawingCenter* DrawingCenter::Default;
    DrawingCenter::DrawingCenter() {
        glGenVertexArrays(1, &__vertexArray);
        glGenBuffers(SWAP_BUFFER, __positionBuffer);

        for (int i = 0; i<SWAP_BUFFER; i++) {
            glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer[i]);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*32, 0, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        glGenBuffers(1, &__extraBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __extraBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*4, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }
    
    DrawingCenter::~DrawingCenter() {
        glDeleteBuffers(SWAP_BUFFER, __positionBuffer);
        glDeleteBuffers(1, &__extraBuffer);
        glDeleteVertexArrays(1, &__vertexArray);
    }
    void DrawingCenter::getInstance() {
        if (!Default) Default = new DrawingCenter();
    }
    void DrawingCenter::Close() {
        if (Default) delete Default;
        Default = 0;
    }
    void DrawingCenter::BindCube(const MyBase::Texture& texture, const glm::vec2& extra) {
        getInstance();
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetCubeShader());
        glBindVertexArray(Default->__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getImageBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, MyBase3D::PointSet::getInstance().getBlockSet());
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, MyBase3D::PointSet::getInstance().getBlockUVS());

        glm::vec4 ex(extra, texture.getSize());
        glBindBuffer(GL_UNIFORM_BUFFER, Default->__extraBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*4, &ex);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, Default->__extraBuffer);
    
        texture.Bind();
    }
    void DrawingCenter::BindSpecialBlock(const MyBase::Texture& texture, const glm::vec2& extra) {
        getInstance();
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetDroppedShader());
        glBindVertexArray(Default->__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getImageBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, MyBase3D::PointSet::getInstance().getBlockSet());
        glBindBufferBase(GL_UNIFORM_BUFFER, 5, MyBase3D::PointSet::getInstance().getBlockUVS());

        glm::vec4 ex(extra, texture.getSize());
        glBindBuffer(GL_UNIFORM_BUFFER, Default->__extraBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*4, &ex);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, Default->__extraBuffer);
    
        texture.Bind();
    }
    void DrawingCenter::BindMargin() {
        getInstance();
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetMarginShader());
    
        glBindVertexArray(Default->__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getMarginBlockIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);
    
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, MyBase3D::PointSet::getInstance().getBlockSet());
    }
    void DrawingCenter::DrawMargins(void* data, const int& size, const MyBase::Color& color, const int& lineWidth) {
        getInstance();
        GLuint COLOR = MyBase::ShapeManager::getInstance().createColor(color);
        glLineWidth(lineWidth);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, COLOR);
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, Default->__positionBuffer[Default->__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, Default->__positionBuffer[Default->__positionBufferPointer]);    
            
            for (int j = 0; j<sz*17; j+=17)
                glDrawArrays(GL_LINE_STRIP, j, 17);
            data = (char*)data + sizeof(GLfloat)*4*sz;
            Default->__positionBufferPointer = (Default->__positionBufferPointer+1)%SWAP_BUFFER;
        }
        MyBase::ShapeManager::getInstance().removeColor(color);
    }
    void DrawingCenter::DrawCubes(void* data, const int& size) {
        getInstance();
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, Default->__positionBuffer[Default->__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::mat4)*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, Default->__positionBuffer[Default->__positionBufferPointer]);

            glDrawArrays(GL_TRIANGLES, 0, 36*sz);
            data = (char*)data + sizeof(glm::mat4)*sz;
            Default->__positionBufferPointer = (Default->__positionBufferPointer+1)%SWAP_BUFFER;
        }
    }
    void DrawingCenter::DrawSpecialBlock(void* data, void* state, const int& size) {
        getInstance();
        GLuint STATE;
        glGenBuffers(1, &STATE);
        glBindBuffer(GL_UNIFORM_BUFFER, STATE);
        int state_size = std::min(size,32)*sizeof(glm::mat4);
        glBufferData(GL_UNIFORM_BUFFER, state_size, 0, GL_DYNAMIC_DRAW);
        for (int i = 0; i<size; i+=32) {
            int sz = std::min(32, size-i);
            glBindBuffer(GL_UNIFORM_BUFFER, Default->__positionBuffer[Default->__positionBufferPointer]);
            glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, Default->__positionBuffer[Default->__positionBufferPointer]);
            
            glBindBuffer(GL_UNIFORM_BUFFER, STATE);
            glBufferSubData(GL_UNIFORM_BUFFER,0, state_size, state);
            glBindBufferBase(GL_UNIFORM_BUFFER, 2, STATE);

            glDrawArrays(GL_TRIANGLES, 0, 36*sz);
            data = (char*)data + sizeof(GLfloat)*4*sz;
            state = (char*)data + sizeof(glm::mat4)*sz;
            Default->__positionBufferPointer = (Default->__positionBufferPointer+1)%SWAP_BUFFER;
        }
        glDeleteBuffers(1, &STATE);
    }
};
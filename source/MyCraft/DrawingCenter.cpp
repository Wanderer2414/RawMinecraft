#include "DrawingCenter.h"
#include "Color.h"
#include "Global.h"
#include "Item.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"
#include "Texture.h"
#include <stdexcept>
namespace MyCraft {

    DrawingStorage::DrawingStorage() {}
    DrawingStorage::~DrawingStorage() {
        clear();
    }

    void DrawingStorage::clear() {
        for (int i = 0; i<__elements.size(); i++) delete __elements[i];
        __elements.clear();
    }
    int DrawingStorage::size() const {
        if (__elements.size()) return (__elements.size()-1)*32+__elements.back()->size;
        else return 0;
    }
    void DrawingStorage::push(const glm::mat4& state, const glm::vec4& info) {
        if (__elements.empty()) __elements.push_back(new Element());
        if (__elements.back()->size==32) __elements.push_back(new Element());
        __elements.back()->state[__elements.back()->size] = state;
        __elements.back()->info[__elements.back()->size] = info;
        __elements.back()->size++;
    }
    void DrawingStorage::setType(const int& index, const BlockCatogary& type) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->info[i].w = type;
    }
    void DrawingStorage::setState(const int& index, const glm::mat4& state) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->state[i] = state;
    }
    void DrawingStorage::remove(const int& index) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        std::swap(__elements[n]->state[i], __elements.back()->state[__elements.back()->size-1]);
        std::swap(__elements[n]->info[i], __elements.back()->info[__elements.back()->size-1]);
        __elements.back()->size--;
        if (!__elements.back()->size) {
            delete __elements.back();
            __elements.pop_back();
        }
    }
    void DrawingStorage::setLight(const int& index, const float& indensity) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->info[i].x = indensity;
    }
    glm::mat4& DrawingStorage::getState(const int& index) {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->state[i];
    }
    ItemType DrawingStorage::getType(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return ItemType(__elements[n]->info[i].w);
    }
    glm::vec3 DrawingStorage::getPosition(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->state[i][3];
    }

    DrawingStorage::Element::Element(): size(0) {}

    DrawingCenter* DrawingCenter::Default;
    DrawingCenter::DrawingCenter() {
        glGenVertexArrays(1, &__vertexArray);
        glGenBuffers(1, &__positionBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, __positionBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*32 + sizeof(glm::vec4)*32, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glGenBuffers(1, &__extraBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, __extraBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*4, 0, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }
    
    DrawingCenter::~DrawingCenter() {
        glDeleteBuffers(1, &__positionBuffer);
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
        glBindBuffer(GL_UNIFORM_BUFFER, Default->__positionBuffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, Default->__positionBuffer);
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
            glBindBuffer(GL_UNIFORM_BUFFER, Default->__positionBuffer);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*4*sz, data);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, Default->__positionBuffer);    
            
            for (int j = 0; j<sz*17; j+=17)
                glDrawArrays(GL_LINE_STRIP, j, 17);
            data = (char*)data + sizeof(GLfloat)*4*sz;
        }
        MyBase::ShapeManager::getInstance().removeColor(color);
    }
    void DrawingCenter::DrawCubes(const DrawingStorage& storage) {
        getInstance();
        for (int i = 0; i<storage.__elements.size(); i++) {
            glBufferSubData(GL_UNIFORM_BUFFER,0,(sizeof(glm::mat4)+sizeof(glm::vec4))*32, storage.__elements[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36*storage.__elements[i]->size);
        }
    }
};
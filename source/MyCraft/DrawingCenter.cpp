#include "DrawingCenter.h"
#include "Color.h"
#include "Global.h"
#include "Item.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "ShapeManager.h"
#include "Texture.h"

namespace MyCraft {

    BlockDrawingStorage::BlockDrawingStorage() {}
    BlockDrawingStorage::~BlockDrawingStorage() {
        clear();
    }

    void BlockDrawingStorage::clear() {
        for (int i = 0; i<__elements.size(); i++) delete __elements[i];
        __elements.clear();
    }
    int BlockDrawingStorage::size() const {
        if (__elements.size()) return (__elements.size()-1)*32+__elements.back()->size;
        else return 0;
    }
    void BlockDrawingStorage::push(const glm::vec3& postiion, const glm::mat4& state, const glm::vec4& info) {
        if (__elements.empty()) __elements.push_back(new Element());
        if (__elements.back()->size==32) __elements.push_back(new Element());
        __elements.back()->state[__elements.back()->size] = state;
        __elements.back()->info[__elements.back()->size] = info;
        __elements.back()->position[__elements.back()->size] = postiion;
        __elements.back()->size++;
    }
    void BlockDrawingStorage::setType(const int& index, const BlockCatogary& type) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->info[i].w = type;
    }
    void BlockDrawingStorage::setState(const int& index, const glm::mat4& state) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->state[i] = state;
    }
    void BlockDrawingStorage::remove(const int& index) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        std::swap(__elements[n]->state[i], __elements.back()->state[__elements.back()->size-1]);
        std::swap(__elements[n]->info[i], __elements.back()->info[__elements.back()->size-1]);
        std::swap(__elements[n]->position[i], __elements.back()->position[__elements.back()->size-1]);
        __elements.back()->size--;
        if (!__elements.back()->size) {
            delete __elements.back();
            __elements.pop_back();
        }
    }
    void BlockDrawingStorage::setLight(const int& index, const float& indensity) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->info[i].x = indensity;
    }
    glm::mat4& BlockDrawingStorage::getState(const int& index) {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->state[i];
    }

    const glm::mat4& BlockDrawingStorage::getState(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->state[i];
    }
    ItemType BlockDrawingStorage::getType(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return ItemType(__elements[n]->info[i].w);
    }
    glm::vec3 BlockDrawingStorage::getPosition(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->position[i];
    }

    BlockDrawingStorage::Element::Element(): size(0) {}

    WaterDrawingStorage::Element::Element(): size(0) {}
    WaterDrawingStorage::WaterDrawingStorage() {}
    WaterDrawingStorage::~WaterDrawingStorage() {
        for (auto i : __elements) delete i;
        __elements.clear();
    }
    int WaterDrawingStorage::size() const {
        if (__elements.size()) return (__elements.size()-1)*32 + __elements.back()->size;
        return 0;
    }
    void WaterDrawingStorage::push(const glm::vec3& position, const glm::vec4& height) {
        if (__elements.empty() || __elements.back()->size == 32) __elements.push_back(new Element());
        __elements.back()->position[__elements.back()->size] = glm::vec4(position, 1);
        __elements.back()->info[__elements.back()->size] = glm::vec4(0);
        __elements.back()->height[__elements.back()->size] = height;
        if (height.x > height.w && height.y > height.z) {
            __elements.back()->info[__elements.back()->size].w = 1;
        }
        if (height.w > height.x && height.w > height.z) {
            __elements.back()->info[__elements.back()->size].w = 1;
        }
        if (height.y>height.x && height.y> height.z) {
            __elements.back()->info[__elements.back()->size].w = 1;
        }
        if (height.w > height.x && height.w>height.z) {
            __elements.back()->info[__elements.back()->size].w = 1;
        }
        __elements.back()->size++;
    }

    glm::vec3 WaterDrawingStorage::getPosition(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __elements[n]->position[i];
    }

    void WaterDrawingStorage::setLight(const int& index, const float& indensity) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        __elements[n]->info[i].x = indensity;
    }

    void WaterDrawingStorage::remove(const int& index) {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        std::swap(__elements[n]->height[i], __elements.back()->height[__elements.back()->size-1]);
        std::swap(__elements[n]->info[i], __elements.back()->info[__elements.back()->size-1]);
        std::swap(__elements[n]->position[i], __elements.back()->position[__elements.back()->size-1]);
        __elements.back()->size--;
        if (!__elements.back()->size) {
            delete __elements.back();
            __elements.pop_back();
        }
    }
    void WaterDrawingStorage::increase() {
        for (int i = 0 ;i<__elements.size(); i++) {
            for (int j = 0; j<__elements[i]->size; j++) {
                __elements[i]->info[j].y = int(__elements[i]->info[j].y+1)%32;
            }
        }
    }
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

    void DrawingCenter::BindWater(const MyBase::Texture& texture) {
        getInstance();
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetWaterShader());
        glBindVertexArray(Default->__vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getRectangle3DIndices());
        glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glBindBufferBase(GL_UNIFORM_BUFFER, 2, MyBase3D::PointSet::getInstance().getRectangle3DSet());

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
    void DrawingCenter::DrawCubes(const BlockDrawingStorage& storage) {
        getInstance();
        for (int i = 0; i<storage.__elements.size(); i++) {
            glBufferSubData(GL_UNIFORM_BUFFER,0,(sizeof(glm::mat4)+sizeof(glm::vec4))*32, storage.__elements[i]);
            glDrawArrays(GL_TRIANGLES, 0, 36*storage.__elements[i]->size);
        }
    }
    void DrawingCenter::DrawWater(const WaterDrawingStorage& storage) {
        getInstance();
        for (int i = 0; i<storage.__elements.size(); i++) {
            glBufferSubData(GL_UNIFORM_BUFFER,0,(3*sizeof(glm::vec4))*32, storage.__elements[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6*storage.__elements[i]->size);
        }
    }
};
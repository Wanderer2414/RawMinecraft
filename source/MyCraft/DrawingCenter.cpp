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
        for (auto i : __side) delete i;
        __side.clear();
        for (auto i:__sideIndices) delete i;
        __sideIndices.clear();
    }

    WaterDrawingStorage::ListSide::ListSide(): size(0) {
        for (int i = 0; i<32; i++) 
            side[i][0] = side[i][1] = side[i][2] = side[i][3] = side[i][4] = side[i][5] = -1;
    }
    int WaterDrawingStorage::size() const {
        if (__sideIndices.size()) return (__sideIndices.size()-1)*32 + __sideIndices.back()->size;
        return 0;
    }
    int WaterDrawingStorage::numberOfSide() const {
        if (__side.size()) return (__side.size()-1)*32 + __side.back()->size;
        return 0;
    }
    void WaterDrawingStorage::push(const glm::vec3& position, const glm::vec4& height) {
        if (__sideIndices.empty() || __sideIndices.back()->size == 32)
            __sideIndices.push_back(new ListSide());
        __sideIndices.back()->heights[__sideIndices.back()->size] = height;
        __sideIndices.back()->position[__sideIndices.back()->size] = position;
        __sideIndices.back()->size++;
    }
    glm::vec4 WaterDrawingStorage::getHeights(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __sideIndices[n]->heights[i];
    }
    void WaterDrawingStorage::setHeight(const int& index, const glm::vec4& height) {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        if (glm::length(__sideIndices[n]->heights[i] - height)>0.1) {
            __sideIndices[n]->heights[i] = height;
            for (int k = 0; k<6; k++) {
                if (__sideIndices[n]->side[i][k]!=-1) {
                    int index = __sideIndices[n]->side[i][k];
                    auto [info, h] = __transformHeight(height, k);
                    __side[index/32]->info[index%32].z = info.z;
                    __side[index/32]->info[index%32].w = info.w;
                    __side[index/32]->heights[index%32] = h;
                }
            }
        }
    }
    std::pair<glm::vec4,glm::vec4> WaterDrawingStorage::__transformHeight(const glm::vec4& h, const unsigned char& plane) const {
        glm::vec4 height= h;
        switch (plane) {
            case 0: {
                glm::vec4 info(0);

                if (height.y>height.x && height.z> height.w) info.w = 1;
                else if (height.x>height.y && height.w > height.z) info.w = 3;
                else if (height.y > height.x && height.y > height.z) info.w = 1;

                if (height.w > height.x && height.z>height.y) info.w = 2;
                else if (height.w > height.x && height.w > height.z) info.w = 1;
                return {info, height};
            }
            break;
            case 1: {
                height.z = height.w = 0;
                return {glm::vec4(0,0,1,0), height};
            }
            break;
            case 2: {
                glm::vec4 info(0, 0, 2, 0);
                height.y = height.z = 0;
                return {info, height};
            }
            break;
            case 3: {
                glm::vec4 info(0, 0, 3, 0);
                height.x = height.y;
                height.y = 0;
                height.w = height.z;
                height.z = 0;
                return {info, height};
            }
            break;
            case 4: {
                glm::vec4 info(0, 0, 4, 0);
                height.y = height.z;
                height.z = 0;
                height.x = height.w;
                height.w = 0;
                return {info, height};
            }
            break;
            case 5: {
                return {glm::vec4(0,0,5,0), glm::vec4(0)};
            }
            break;
        }
        return {glm::vec4(0), glm::vec4(0)};
    }
    void WaterDrawingStorage::pushSide(const int& index, const unsigned char& plane, const float& lightness) {
        if (index>=size() || plane<0 || plane>5) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        if (__sideIndices[n]->side[i][plane]!=-1) return ;
        
        auto [info, height] = __transformHeight(__sideIndices[n]->heights[i], plane);

        info.x = lightness;
        __sideIndices[n]->side[i][plane] = numberOfSide();
        __add_side(__sideIndices[n]->position[i], info, height);
        __side.back()->indices[__side.back()->size-1] = index*6+plane;
    }
    void WaterDrawingStorage::__add_side(const glm::vec3& position, const glm::vec4& info, const glm::vec4& height) {
        if (__side.empty() || __side.back()->size == 32) __side.push_back(new Element());
        
        __side.back()->position[__side.back()->size] = glm::vec4(position, 1);
        __side.back()->info[__side.back()->size] = info;
        __side.back()->heights[__side.back()->size] = height;
        
        __side.back()->size++;
    }
    void WaterDrawingStorage::removeSide(const int& index, const unsigned char& plane) {
        if (index>=size() || plane<0 || plane>5) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        if (__sideIndices[n]->side[i][plane] != -1) {
            __remove_side(__sideIndices[n]->side[i][plane]);
            __sideIndices[n]->side[i][plane] = -1;
        }
    }
    void WaterDrawingStorage::__remove_side(const int& index) {
        if (index>=numberOfSide()) 
            throw std::runtime_error("Out of range!");
        int n = index/32, i = index%32;
        if (index < numberOfSide()-1) {
            std::swap(__side[n]->indices[i], __side.back()->indices[__side.back()->size-1]);
            std::swap(__side[n]->heights[i], __side.back()->heights[__side.back()->size-1]);
            std::swap(__side[n]->info[i], __side.back()->info[__side.back()->size-1]);
            std::swap(__side[n]->position[i], __side.back()->position[__side.back()->size-1]);
            int blockIndex = __side[n]->indices[i];
            int n = blockIndex/6/32, i = blockIndex/6%32;
            __sideIndices[n]->side[i][blockIndex%6] = index;
        }
        __side.back()->size--;
        if (!__side.back()->size) __side.pop_back();
    }

    glm::vec3 WaterDrawingStorage::getPosition(const int& index) const {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        return __sideIndices[n]->position[i];
    }

    void WaterDrawingStorage::setLight(const int& index, const float& indensity) {
        if (index>=size()) return ;
        int i = index%32, n = index/32;
        for (int k = 0; k<6; k++) {
            if (__sideIndices[n]->side[i][k]!=-1) {
                int index = __sideIndices[n]->side[i][k];
                __side[index/32]->info[index%32].x = indensity;
            }
        }
    }

    void WaterDrawingStorage::remove(const int& index) {
        if (index>=size()) 
            throw std::runtime_error("Out of range!");
        int i = index%32, n = index/32;
        for (int k = 0; k<6; k++) {
            if (__sideIndices[n]->side[i][k]!=-1) removeSide(index, k);
        }
        if (index!=size()-1) {
            std::swap(__sideIndices[n]->side[i], __sideIndices.back()->side[__sideIndices.back()->size-1]);
            std::swap(__sideIndices[n]->heights[i], __sideIndices.back()->heights[__sideIndices.back()->size-1]);
            std::swap(__sideIndices[n]->position[i], __sideIndices.back()->position[__sideIndices.back()->size-1]);
            for (int k = 0; k<6; k++) {
                if (__sideIndices[n]->side[i][k]!=-1) {
                    int sideIndex = __sideIndices[n]->side[i][k];
                    __side[sideIndex/32]->indices[sideIndex%32] = index*6+k;
                }
            }
        }
        __sideIndices.back()->size--;
        if (!__sideIndices.back()->size) {
            delete __sideIndices.back();
            __sideIndices.pop_back();
        }
    }
    void WaterDrawingStorage::increase() {
        for (int i = 0 ;i<__side.size(); i++) {
            for (int j = 0; j<__side[i]->size; j++) {
                __side[i]->info[j].y = int(__side[i]->info[j].y+1)%32;
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
        for (int i = 0; i<storage.__side.size(); i++) {
            glBufferSubData(GL_UNIFORM_BUFFER,0,(3*sizeof(glm::vec4))*32, storage.__side[i]);
            glDrawArrays(GL_TRIANGLES, 0, 6*storage.__side[i]->size);
        }
    }
};
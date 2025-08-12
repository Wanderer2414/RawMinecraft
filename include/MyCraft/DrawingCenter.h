#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
#include "Color.h"
#include "Item.h"
#include "Texture.h"
#include "glm/fwd.hpp"
#include <memory>
namespace MyCraft {
    class DrawingStorage {
    public:
        DrawingStorage();
        ~DrawingStorage();

        int size() const;
        void clear();
        void push(const glm::mat4&, const glm::vec4&);
        void remove(const int& index);
        void setLight(const int& index, const float& indensity);
        void setType(const int& index, const BlockCatogary& type);
        void setState(const int& index, const glm::mat4& state);
        ItemType getType(const int& index) const;
        glm::vec3 getPosition(const int& index) const;
        glm::mat4& getState(const int& index);
        const glm::mat4& getState(const int& index) const;
        friend class DrawingCenter;
    private:
        struct Element {    
            Element();
            glm::mat4    state[32];
            glm::vec4    info[32];
            unsigned char size;
        };
        std::vector<Element*> __elements;
    };
    class DrawingCenter {
    public:
        #define SWAP_BUFFER 3
        static void BindCube(const MyBase::Texture& texture, const glm::vec2& extra = {1,1});
        static void BindMargin();
        static void DrawCubes(const DrawingStorage& store);
        static void DrawMargins(void* data, const int& size, const MyBase::Color& color, const int& lineWidth);
        static void Close();
    private:
        static void getInstance();
        ~DrawingCenter();
        DrawingCenter();
        static DrawingCenter* Default;
        GLuint __vertexArray, __positionBuffer, __extraBuffer;
    };
};
#endif
#ifndef DRAWING_CENTER_H
#define DRAWING_CENTER_H
#include "Block.h"
#include "Color.h"
#include "Item.h"
#include "Texture.h"
#include "glm/fwd.hpp"
#include <memory>
namespace MyCraft {
    class BlockDrawingStorage {
    public:
        BlockDrawingStorage();
        ~BlockDrawingStorage();

        int size() const;
        void clear();
        void push(const glm::vec3& position, const glm::mat4&, const glm::vec4&);
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
            glm::vec3   position[32];
            unsigned char size;
        };
        std::vector<Element*> __elements;
    };
    class WaterDrawingStorage {
    public:
        WaterDrawingStorage();
        ~WaterDrawingStorage();
        int size() const;
        int numberOfSide() const;
        WaterDrawingStorage(const WaterDrawingStorage&) = delete;
        WaterDrawingStorage& operator=(const WaterDrawingStorage&) const = delete; 
        glm::vec3 getPosition(const int& index) const;
        glm::vec4 getHeights(const int& index) const;
        void increase();
        void push(const glm::vec3& position, const glm::vec4& height);
        void setLight(const int& index, const float& indensity);
        void pushSide(const int& index, const unsigned char& plane,const float& lightness);
        void remove(const int& index);
        void removeSide(const int& index, const unsigned char& plane);
        void setHeight(const int& index, const glm::vec4& height);
        friend class DrawingCenter;
    protected:
    private:
        struct Element {
            Element();
            glm::vec4  position[32];
            glm::vec4  info[32];
            glm::vec4  heights[32];
            int        indices[32];
            unsigned int size;
        };
        struct ListSide {
            ListSide();
            int         side[32][6];
            glm::vec3   position[32];
            glm::vec4   heights[32];
            unsigned int size;
        };
        std::pair<glm::vec4, glm::vec4> __transformHeight(const glm::vec4& height, const unsigned char& plane) const;
        std::vector<ListSide*> __sideIndices;
        std::vector<Element*> __side;
        void __add_side(const glm::vec3& position, const glm::vec4& info, const glm::vec4& height);
        void __remove_side(const int& index);
    };
    class DrawingCenter {
    public:
        #define SWAP_BUFFER 3
        static void BindCube(const MyBase::Texture& texture, const glm::vec2& extra = {1,1});
        static void BindMargin();
        static void DrawCubes(const BlockDrawingStorage& store);
        static void DrawMargins(void* data, const int& size, const MyBase::Color& color, const int& lineWidth);
        static void BindWater(const MyBase::Texture& texture);
        static void DrawWater(const WaterDrawingStorage& store);
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
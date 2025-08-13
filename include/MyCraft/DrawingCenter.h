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
        WaterDrawingStorage(const WaterDrawingStorage&) = delete;
        WaterDrawingStorage& operator=(const WaterDrawingStorage&) const = delete; 
        void push(const glm::vec3& position, const glm::vec4& height, const float& lightness);
        friend class DrawingCenter;
    protected:
    private:
        struct Element {
            Element();
            glm::vec4  position[32];
            glm::vec4  info[32];
            glm::vec4  height[32];
            unsigned int size;
        };
        std::vector<Element*> __elements;
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
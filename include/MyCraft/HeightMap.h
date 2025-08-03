#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H
#include "Global.h"
namespace MyCraft {
    class HeightMap {
    public:
        HeightMap(const unsigned int& width, const unsigned int& height, const int& z);
        ~HeightMap();
        HeightMap(const HeightMap&) = delete;
        HeightMap& operator=(const HeightMap&) const = delete; 
        bool isValid(const glm::ivec2& position) const;
        bool isShow(const glm::ivec3& position) const;
        const glm::ivec2& getSize() const;
        const glm::ivec2& getPosition() const;
        int& operator[](const glm::ivec2&);
        const int& operator[](const glm::ivec2&) const;
        void setPosition(const glm::ivec2& position);
    protected:
    private:
        glm::ivec2  __size, __position;
        int** __map;
    };
}
#endif
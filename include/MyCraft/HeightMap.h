#ifndef HEIGHT_MAP_H
#define HEIGHT_MAP_H
#include "Global.h"
namespace MyCraft {
    class HeightMap {
    public:
        HeightMap(const unsigned int& width, const unsigned int& height);
        ~HeightMap();
        HeightMap(const HeightMap&) = delete;
        HeightMap& operator=(const HeightMap&) const = delete; 

        const glm::ivec2& getSize() const;
        unsigned int& getHeight(const int& x, const int& y);
        const unsigned int& getHeight(const int& x, const int& y) const;
    protected:
    private:
        glm::ivec2  __size;
        unsigned int** __map;
    };
}
#endif
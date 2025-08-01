#include "HeightMap.h"
namespace MyCraft {
    HeightMap::HeightMap(const unsigned int& width, const unsigned int& height): __size(width, height) {
        __map = new unsigned int*[width];
        for (int i = 0; i<width; i++) {
            __map[i] = new unsigned int[height];
            memset(__map[i], 0 ,height*sizeof(int));
        }
    }
    HeightMap::~HeightMap() {
        for (int i = 0; i<__size.x; i++) delete[] __map[i];
        delete[] __map;
        __map = 0;
    }
    const glm::ivec2& HeightMap::getSize() const {
        return __size;
    }
    unsigned int& HeightMap::getHeight(const int& x, const int& y) {
        if (x<0 || x>=__size.x || y<0 || y>= __size.y)
            throw std::runtime_error("Out range of height map!");
        return __map[x][y];
    }
    const unsigned int& HeightMap::getHeight(const int& x, const int& y) const {
        if (x<0 || x>=__size.x || y<0 || y>= __size.y)
            throw std::runtime_error("Out range of height map!");
        return __map[x][y];
    }
}
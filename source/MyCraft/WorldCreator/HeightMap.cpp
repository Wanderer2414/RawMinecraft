#include "HeightMap.h"
namespace MyCraft {
    HeightMap::HeightMap(const unsigned int& width, const unsigned int& height, const int& z): __size(width, height), __position(0, 0) {
        __map = new int*[width];
        for (int i = 0; i<width; i++) {
            __map[i] = new int[height];
            for (int j = 0; j<height; j++) __map[i][j] = z;
        }
    }
    HeightMap::~HeightMap() {
        for (int i = 0; i<__size.x; i++) delete[] __map[i];
        delete[] __map;
        __map = 0;
    }
    bool HeightMap::isValid(const glm::ivec2& position) const {
        if (position.x<__position.x || position.x>=__position.x+__size.x || position.y<__position.y || position.y>= __position.y + __size.y)
            return false;
        return true;
    }
    bool HeightMap::isShow(const glm::ivec3& position) const {
        glm::ivec2 offset(position.x-__position.x, position.y-__position.y);
        if (offset.x<0 || offset.x>=__size.x || offset.y<0 || offset.y>= __size.y)
            throw std::runtime_error("Out range of height map!");
        if (offset.x == 0 || offset.x == __size.x-1 || offset.y == 0 || offset.y == __size.y-1)
            return true;
        if (__map[offset.x-1][offset.y]<position.z || __map[offset.x+1][offset.y]<position.z ||
            __map[offset.x][offset.y-1]<position.z || __map[offset.x][offset.y+1]<position.z || 
            position.z == __map[offset.x][offset.y]) return true;
        return false; 
    }
    const glm::ivec2& HeightMap::getSize() const {
        return __size;
    }
    const glm::ivec2& HeightMap::getPosition() const {
        return __position;
    }
    int& HeightMap::operator[](const glm::ivec2& position) {
        glm::ivec2 offset(position - __position);
        if (offset.x<0 || offset.x>=__size.x || offset.y<0 || offset.y>= __size.y)
            throw std::runtime_error("Out range of height map!");
        return __map[offset.x][offset.y];
    }
    const int& HeightMap::operator[](const glm::ivec2& position) const {
        glm::ivec2 offset(position - __position);
        if (offset.x<0 || offset.x>=__size.x || offset.y<0 || offset.y>= __size.y)
            throw std::runtime_error("Out range of height map!");
        return __map[offset.x][offset.y];
    }
    void HeightMap::setPosition(const glm::ivec2& position) {
        __position = position;
    }
}
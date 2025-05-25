#include "World.h"

World::World(const int& x, const int& y, const int& z): pPosition(x-world_side*16, y-world_side*16, z-world_side*16) {
    for (int i = 0; i<world_side*2+1; i++) {
        for (int j = 0; j<world_side*2+1; j++) {
            for (int k = 0; k<world_side*2+1; k++) {
                pChunks[i][j][k].setPosition(pPosition.x+i*16, pPosition.y+j*16, pPosition.z+k*16);
            }
        }
    }
}
World::~World() {
}
char& World::at(const int& x, const int& y, const int& z) {
    int rX = (x-pPosition.x);
    int rY = (y-pPosition.y);
    int rZ = (z-pPosition.z);
    return pChunks[rX/16][rY/16][rZ/16].at(rX%16, rY%16, rZ%16);
}
void World::glDraw() const {
    for (int i = 0; i<world_side*2+1; i++) {
        for (int j = 0; j<world_side*2+1; j++) {
            for (int k = 0; k<world_side*2+1; k++) {
                pChunks[i][j][k].glDraw();
            }
        }
    }
}
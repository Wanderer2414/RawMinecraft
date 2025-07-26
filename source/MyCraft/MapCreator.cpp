#include "MapCreator.h"
#include "Block.h"
#include "Chunk.h"
#include "Global.h"
#include "Tectonic.h"
#include "General.h"
namespace MyCraft {
    std::string MapCreator::getFileName(const std::string& src, const glm::ivec3& position) {
        size_t index = (position.x+500)*1000*1000 + (position.y+500)*1000 + (500 + position.z);
        return src + std::to_string(index)+".bin";
    }
    void MapCreator::createBedrockLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound) {
        std::thread* threads[100];
        memset(&threads[0], 0, sizeof(void*)*100);
        int index = 0;
        for (int i = 4; i<6; i++) {
            for (int j = 4; j<6; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-500, (j+1)*100 - 500);
                threads[index++] = new std::thread(createSubBedrockLayer, mtx, percent, total/4, src, xBound, yBound, heightBound);
            }
        }
        for (int i = 0; i<100; i++) {
            if (threads[i]) {
                threads[i]->join();
                delete threads[i];
            }
        }
    }
    void MapCreator::createSubBedrockLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                glm::ivec3 origin(x,y,height);
                std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                glm::vec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::vec3));
                unsigned int size = 0;
                file.write((char*)&size, sizeof(int));

                size = 256;
                file.write((char*)&size, sizeof(int));
                BlockCatogary::Catogary types[4096];
                memset(&types[0], 0, sizeof(BlockCatogary::Catogary)*4096);
                for (int i = 0; i<256; i++) types[i*16 + 15] = BlockCatogary::Grass;
                file.write((char*)&types, sizeof(BlockCatogary::Catogary)*4096);
                file.close();
                std::lock_guard<std::mutex> lock(*mtx);
                *percent += one_part*total;
            }
        }
    }

    void MapCreator::createMagmaLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound) {
        std::thread* threads[100];
        memset(&threads[0], 0, sizeof(void*)*100);
        int index = 0;
        for (int i = 4; i<6; i++) {
            for (int j = 4; j<6; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-500, (j+1)*100 - 500);
                threads[index++] = new std::thread(createSubMagmaLayer, mtx, percent, total/4, src, xBound, yBound, heightBound);
            }
        }
        for (int i = 0; i<100; i++) {
            if (threads[i]) {
                threads[i]->join();
                delete threads[i];
            }
        }
    }
    void MapCreator::createSubMagmaLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                glm::ivec3 origin(x,y,height);
                std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                glm::vec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::vec3));
                unsigned int size = 1;
                file.write((char*)&size, sizeof(int));

                BlockCatogary::Catogary type = BlockCatogary::Catogary::Dirt;
                file.write((char*)&type, sizeof(BlockCatogary::Catogary));
                size = 512;
                file.write((char*)&size, sizeof(int));
                unsigned int bits[128];
                for (int i = 0; i<128; i++) bits[i] = 34<<16 | 34;
                file.write((char*)&bits[0], 128*sizeof(int));
                size = 3328;
                file.write((char*)&size, sizeof(int));
                BlockCatogary::Catogary types[4096];
                memset(&types[0], BlockCatogary::Dirt, sizeof(BlockCatogary::Catogary)*4096);
                for (int i = 0; i<256; i++) {
                    types[i*16+2] = types[i*16+3] = types[i*16+4] = BlockCatogary::Air;
                }
                file.write((char*)&types, sizeof(BlockCatogary::Catogary)*4096);
                file.close();
                std::lock_guard<std::mutex> lock(*mtx);
                *percent += one_part*total;
            }
        }
    }

    void MapCreator::createTopSoilLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& zBound) {
        std::thread* threads[100];
        memset(&threads[0], 0, sizeof(void*)*100);
        int index = 0;
        for (int i = 4; i<6; i++) {
            for (int j = 4; j<6; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-500, (j+1)*100 - 500);
                threads[index++] = new std::thread(createSubTopSoilLayer, mtx, percent, total/4, src, xBound, yBound, zBound);
            }
        }
        for (int i = 0; i<100; i++) {
            if (threads[i]) {
                threads[i]->join();
                delete threads[i];
            }
        }
    }
    void MapCreator::createSubTopSoilLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x)*(zBound.y - zBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                for (int z = zBound.x; z < zBound.y; z++) {
                    glm::ivec3 origin(x,y,z);
                    std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                    glm::vec3 position = origin*16;
                    file.write((char*)&position, sizeof(glm::vec3));
                    unsigned int size = 1;
                    file.write((char*)&size, sizeof(int));

                    BlockCatogary::Catogary type = BlockCatogary::Catogary::Dirt;
                    file.write((char*)&type, sizeof(BlockCatogary::Catogary));
                    size = 256;
                    file.write((char*)&size, sizeof(int));

                    unsigned int bits[128];
                    for (int i = 0; i<128; i++) bits[i] = 1<<15 | 1 << 31;
                    file.write((char*)&bits[0], 128*sizeof(int));

                    size = 4096;
                    file.write((char*)&size, sizeof(int));
                    BlockCatogary::Catogary types[4096];
                    memset(&types[0], BlockCatogary::Dirt, sizeof(BlockCatogary::Catogary)*4096);

                    file.write((char*)&types, sizeof(BlockCatogary::Catogary)*4096);
                    file.close();
                    std::lock_guard<std::mutex> lock(*mtx);
                    *percent += one_part*total;
                }
            }
        }
    }
    void MapCreator::createSubTemperateZone(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::vec2& bound, unsigned char** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::vec3& origin) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                int maxHeight = 0;
                glm::vec3 position(x + origin.x,y + origin.y, origin.z);
                bool isTaller = true;
                while (isTaller) {
                    isTaller = false;
                    Chunk chunk;
                    chunk.__isChange = true;
                    position.z = origin.z + maxHeight/16.f;
                    chunk.__source = getFileName(src, position);
                    memset(chunk.__blockTypes, BlockCatogary::Air, 4096*sizeof(BlockCatogary::Catogary));
                    chunk.__position = 16.f*position;
                    for (int i = 0; i<16; i++) {
                        for (int j = 0; j<16; j++) {
                            int mX = x*16+i, mY = y*16+j;
                            if (board[mX][mY]>maxHeight) {
                                int mZ = board[mX][mY]-maxHeight-1;
                                if (mZ>=16) isTaller = true;
                                else {
                                    chunk.__bits[i][j][mZ] = 1;
                                    chunk.__list[BlockCatogary::Grass].push_back(glm::vec4(chunk.__position+glm::vec3(i,j,mZ),1));
                                    chunk.__blockTypes[i][j][mZ] = BlockCatogary::Grass;
                                    chunk.__numBlock++;
                                }
                                for (int z = 0; z<=std::min(mZ, 15); z++) {
                                    if (mX==0 || mX==bound.x-1 || mX == 0 || mY ==bound.y-1) {
                                        chunk.__bits[i][j][z] = 1;
                                        chunk.__list[BlockCatogary::Grass].push_back(glm::vec4(chunk.__position+glm::vec3(i,j,z),1));
                                    }
                                    else if (board[mX-1][mY]<z+maxHeight+1 || board[mX+1][mY]<z+maxHeight+1 || board[mX][mY-1]<z+maxHeight+1 || board[mX][mY+1]<z+maxHeight+1) {
                                        chunk.__bits[i][j][z] = 1;
                                        chunk.__list[BlockCatogary::Grass].push_back(glm::vec4(chunk.__position+glm::vec3(i,j,z),1));
                                    }
                                    chunk.__numBlock++;
                                    chunk.__blockTypes[i][j][z] = BlockCatogary::Grass;
                                }
                            }
                        }
                    }
                    chunk.save();
                    if (isTaller) maxHeight+=16;
                }
                std::lock_guard<std::mutex> lock(*mtx);
                *percent += one_part*total;
            }
        }
    }
    void MapCreator::createTemperateZone(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::vec2& yBound, const float& z) {
        Area area(10, {0,0}, {3200, 1600});
        unsigned char *map[3200];
        for (int i = 0; i<3200; i++) map[i] = new unsigned char[1600];
        area.draw({3200, 1600}, map);
        glm::vec2 bound(3200, 1600);
        std::thread* threads[100];
        int index = 0;
        glm::vec3 origin(-100, -50, z);
        for (int i = 0; i<10; i++) {
            for (int j = 0; j<10; j++) {
                glm::vec2 xBound(i*20, (i+1)*20), yBound(j*10, (j+1)*10);
                threads[index++] = new std::thread(createSubTemperateZone, mtx, percent, total/100, src, bound, map, xBound, yBound, origin);
            }
        }
        for (int i = 0; i<100; i++) {
            threads[i]->join();
            delete threads[i];
        }
        for (int i = 0; i<3200; i++) delete[] map[i];
    }

    void MapCreator::create(std::mutex* mtx, double* percent, const std::string& src) {
        // createBedrockLayer(src, -1);
        createBedrockLayer(mtx, percent, 0.05, src, -10);
        createMagmaLayer(mtx, percent, 0.05, src, -9);
        createTopSoilLayer(mtx, percent, 0.1, src, {-8, -6});
        createTemperateZone(mtx, percent, 0.8, src, {0,0}, -6);
    }
}
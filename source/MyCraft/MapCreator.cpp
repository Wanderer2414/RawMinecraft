#include "MapCreator.h"
#include "Block.h"
#include "Chunk.h"
#include "Color.h"
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
        for (int i = 0; i<10; i++) {
            for (int j = 0; j<5; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-250, (j+1)*100 - 250);
                threads[index++] = new std::thread(createSubBedrockLayer, mtx, percent, total/50, src, xBound, yBound, heightBound);
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
                glm::ivec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::ivec3));
                unsigned int size = 0;
                file.write((char*)&size, sizeof(int));

                size = 256;
                file.write((char*)&size, sizeof(int));
                BlockCatogary types[4096];
                memset(&types[0], 0, sizeof(BlockCatogary)*4096);
                for (int i = 0; i<256; i++) types[i*16 + 15] = BlockCatogary::Grass;
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
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
        for (int i = 0; i<10; i++) {
            for (int j = 0; j<5; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-250, (j+1)*100 - 250);
                threads[index++] = new std::thread(createSubMagmaLayer, mtx, percent, total/50, src, xBound, yBound, heightBound);
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
                glm::ivec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::ivec3));
                unsigned int size = 512;
                file.write((char*)&size, sizeof(int));

                unsigned int bits[128];
                for (int i = 0; i<128; i++) bits[i] = 34<<16 | 34;
                file.write((char*)&bits[0], 128*sizeof(int));
                size = 3328;
                file.write((char*)&size, sizeof(int));
                BlockCatogary types[4096];
                memset(&types[0], BlockCatogary::Dirt, sizeof(BlockCatogary)*4096);
                for (int i = 0; i<256; i++) {
                    types[i*16+2] = types[i*16+3] = types[i*16+4] = BlockCatogary::Air;
                }
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
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
        for (int i = 0; i<10; i++) {
            for (int j = 0; j<5; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-250, (j+1)*100 - 250);
                threads[index++] = new std::thread(createSubTopSoilLayer, mtx, percent, total/50, src, xBound, yBound, zBound);
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
                    glm::ivec3 position = origin*16;
                    file.write((char*)&position, sizeof(glm::ivec3));
                    unsigned int size = 256;
                    file.write((char*)&size, sizeof(int));

                    unsigned int bits[128];
                    for (int i = 0; i<128; i++) bits[i] = 1<<15 | 1 << 31;
                    file.write((char*)&bits[0], 128*sizeof(int));

                    size = 4096;
                    file.write((char*)&size, sizeof(int));
                    BlockCatogary types[4096];
                    memset(&types[0], BlockCatogary::Dirt, sizeof(BlockCatogary)*4096);

                    file.write((char*)&types, sizeof(BlockCatogary)*4096);
                    file.close();
                    std::lock_guard<std::mutex> lock(*mtx);
                    *percent += one_part*total;
                }
            }
        }
    }
    void MapCreator::createSubTemperateZone(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::vec2& bound, unsigned char** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                int maxHeight = 0;
                glm::ivec3 position(x + origin.x,y + origin.y, origin.z);
                bool isTaller = true;
                float averageHeight = 0;
                while (isTaller) {
                    isTaller = false;
                    Chunk chunk;
                    chunk.__isChange = true;
                    position.z = origin.z + maxHeight/16.f;
                    chunk.__source = getFileName(src, position);
                    memset(chunk.__blockTypes, BlockCatogary::Air, 4096*sizeof(BlockCatogary));
                    chunk.__position = 16*position;
                    for (int i = 0; i<16; i++) {
                        for (int j = 0; j<16; j++) {
                            int mX = x*16+i, mY = y*16+j;
                            if (board[mX][mY]>maxHeight) {
                                int mZ = board[mX][mY]-maxHeight-1;
                                if (mZ>=16) isTaller = true;
                                else {
                                    chunk.__bits[i][j][mZ] = 1;
                                    chunk.__list.push_back(glm::vec4(chunk.__position+glm::ivec3(i,j,mZ),Grass));
                                    chunk.__blockTypes[i][j][mZ] = Grass;
                                    chunk.__numBlock++;
                                    averageHeight += 1.0f*(mZ+maxHeight+1)/256;
                                }
                                for (int z = 0; z<=std::min(mZ, 15); z++) {
                                    if (mX==0 || mX==bound.x-1 || mX == 0 || mY ==bound.y-1) {
                                        chunk.__bits[i][j][z] = 1;
                                        chunk.__list.push_back(glm::vec4(chunk.__position+glm::ivec3(i,j,z),Grass));
                                    }
                                    else if (board[mX-1][mY]<z+maxHeight+1 || board[mX+1][mY]<z+maxHeight+1 || board[mX][mY-1]<z+maxHeight+1 || board[mX][mY+1]<z+maxHeight+1) {
                                        chunk.__bits[i][j][z] = 1;
                                        chunk.__list.push_back(glm::vec4(chunk.__position+glm::ivec3(i,j,z),Grass));
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
    void MapCreator::RenderToMap(MyBase::Color* texture, unsigned char** map, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& origin) {
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                float colorAvarage = 0;
                for (int i = 0; i<16; i++) {
                    for (int j = 0; j<16; j++) colorAvarage+=1.0*map[x*16 + i][y*16 + j]/256;;
                }
                unsigned char color = colorAvarage;
                if (color) {
                    MyBase::Color& c = texture[(y + origin.y + 250)*1000 + x + origin.x + 500];
                    c.red = c.blue = c.green = color;
                    c.alpha = 255;
                }
            }
        }
    }
    void MapCreator::createTemperateZone(std::mutex* mtx, double* percent, const double& total, MyBase::Color* texture, const std::string& src, const glm::vec2& yBound, const float& z) {
        srand(clock());
        unsigned char count = rand()%3+2;
        int xMax = 1000, xPart = xMax/count;
        for (int k = 0; k<count; k++) {
            float p = 1.0f*(rand()%100 + 50)/100;
            int xSize = 0;
            if (k==count-1) {
                xSize = xMax;
                p = 1.0f*xSize/xPart;
                xMax = 0;
            }
            else {
                xSize = p*xPart;
                xMax -= xSize;
            }
            unsigned char **map = new unsigned char*[xSize*16];
            for (int i = 0; i<xSize*16; i++) {
                map[i] = new unsigned char[1600];
                memset(map[i], 0, 1600);
            }
            Area area(p*(rand()%10+5), {0,100}, {xSize*16, 1400});
            glm::vec2 bound(xSize*16, 1600);
            area.draw(bound, map);
            std::thread* threads[100];
            
            glm::ivec3 origin(500-xMax-xSize, -50, z);
            for (int i = 0; i<100; i++) {
                glm::vec2 xBound(0, xSize), yBound(i, i+1);
                threads[i] = new std::thread(createSubTemperateZone, mtx, percent, total/100/count, src, bound,  map, xBound, yBound, origin);
            }
            RenderToMap(texture, map, {0, xSize}, {0, 100}, {500-xSize-xMax, -50});
            
            for (int i = 0; i<100; i++) {
                threads[i]->join();
                delete threads[i];
            }
            for (int i = 0; i<xSize; i++) delete[] map[i];
            delete[] map;
        }
    }

    void MapCreator::create(std::mutex* mtx, double* percent, const std::string& src) {
        // createBedrockLayer(src, -1);
        MyBase::Color color[500][1000];
        for (int i = 0; i<500; i++) {
            for (int j = 0; j<1000; j++) color[i][j] = BLUE;
        }
        createBedrockLayer(mtx, percent, 0.2, src, -10);
        createMagmaLayer(mtx, percent, 0.2, src, -9);
        createTopSoilLayer(mtx, percent, 0.2, src, {-8, -6});
        createTemperateZone(mtx, percent, 0.4, &color[0][0], src, {0,0}, -6);
        stbi_write_png((src+"overal.png").c_str(), 1000, 500, 4, &color[0][0], sizeof(char)*4*1000);
    }
}
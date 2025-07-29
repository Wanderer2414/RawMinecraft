#include "MapCreator.h"
#include "Block.h"
#include "Chunk.h"
#include "Color.h"
#include "File.h"
#include "Global.h"
#include "Tectonic.h"

namespace MyCraft {
    std::string MapCreator::getFileName(const std::string& src, const glm::ivec3& position) {
        size_t index = (position.x+500)*1000*1000 + (position.y+500)*1000 + (500 + position.z);
        return src + std::to_string(index)+".bin";
    }
    void MapCreator::createBedrockLayer(double* percent, const double& total, const std::string& src, const int& heightBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubBedrockLayer, percent, total/2, src, xBound, yBound, heightBound);

        xBound= {0, 500};
        std::thread threadA(createSubBedrockLayer, percent, total/2, src, xBound, yBound, heightBound);

        thread.join();
        threadA.join();
    }
    void MapCreator::createSubBedrockLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
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
                for (int i = 0; i<256; i++) types[i*16 + 15] = BedRock;
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createMagmaLayer(double* percent, const double& total, const std::string& src, const int& heightBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubMagmaLayer,percent, total/2, src, xBound, yBound, heightBound);
        xBound = {0, 500};
        std::thread threadA(createSubMagmaLayer, percent, total/2, src, xBound, yBound, heightBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubMagmaLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
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
                memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
                for (int i = 0; i<256; i++) {
                    types[i*16+1] = types[i*16+5] = Obsidian;
                    types[i*16+2] = types[i*16+3] = types[i*16+4] = Air;
                }
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& zBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubTopSoilLayer, percent, total/2, src, xBound, yBound, zBound);

        xBound = {0, 500};
        std::thread threadA(createSubTopSoilLayer, percent, total/2, src, xBound, yBound, zBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
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
                    memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
                    for (int i = 0; i<256; i++) types[i*16+15] = Sand;

                    file.write((char*)&types, sizeof(BlockCatogary)*4096);
                    file.close();
                }
                *percent += one_part*total;
            }
        }
    }
    void MapCreator::toBiome(Biome**& biome, unsigned int** board, const glm::ivec2& size) {
        biome = new Biome*[size.x];
        for (int i = 0; i<size.x; i++) {
            biome[i] = new Biome[size.y];
            memset(biome[i], (int)Biome::Sea, size.y);
        }
        for (int x = 1; x<size.x-1; x++) {
            for (int y = 1; y<size.y-1; y++) {
                float height = 0;
                for (int i = 0; i<16; i++) {
                    for (int j = 0; j<16; j++) {
                        int mX = x*16+i, mY = y*16+j;
                        height += 1.0f*board[mX][mY]/256;
                    }
                }
                if (height>72)  biome[x][y] = Biome::High;
                else if (height>64) biome[x][y] = Biome::Mid;
                else if (height>32) biome[x][y] = Biome::Low;
                else biome[x][y] = Biome::Sea;
            }
        }
    }
    void MapCreator::createSubTemperateZone(double* percent, const double& total, Biome** biome, MyBase::Color* color, const std::string& src, const glm::vec2& bound, unsigned int** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin) {
        double one_part = 1.0/((xBound.y-xBound.x));
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                int maxHeight = 0;
                glm::ivec3 position(x + origin.x,y + origin.y, origin.z);
                bool isTaller = true;
                while (isTaller) {
                    isTaller = false;
                    position.z = origin.z + maxHeight/16.f;
                    Chunk* chunk = Chunk::Load(src, position);
                    chunk->__isChange = true;
                    for (int i = 0; i<16; i++) {
                        for (int j = 0; j<16; j++) {
                            int mX = x*16+i, mY = y*16+j;
                            if (board[mX][mY]>maxHeight) {
                                int mZ = board[mX][mY]-maxHeight-1;
                                if (mZ>=16) isTaller = true;
                                for (int z = 0; z<=std::min(mZ, 15); z++) {
                                    if (mX==0 || mX==bound.x-1 || mX == 0 || mY ==bound.y-1) {
                                        chunk->__bits[i][j][z] = 1;
                                        chunk->__numBit++;
                                    }
                                    else if (board[mX-1][mY]<z+maxHeight+1 || board[mX+1][mY]<z+maxHeight+1 || board[mX][mY-1]<z+maxHeight+1 || board[mX][mY+1]<z+maxHeight+1) {
                                        chunk->__bits[i][j][z] = 1;
                                        chunk->__numBit++;
                                    }
                                    chunk->__numBlock++;
                                    chunk->__blockTypes[i][j][z] = Stone;
                                }
                                if (mZ<16 && !chunk->__bits[i][j][mZ]) {
                                    chunk->__bits[i][j][mZ] = 1;
                                    chunk->__numBit++;
                                }
                                if (mZ < 16) {
                                    if (biome[x][y] == Biome::Desert || biome[x][y] == Biome::Beach) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->__blockTypes[i][j][z] = Sand;
                                    }
                                    else if (biome[x][y] == Biome::MixRockyHill && mZ < 10) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->__blockTypes[i][j][z] = Sand;
                                    }
                                    else if (biome[x][y] == Biome::MixOasis) {
                                        int rate = rand()%5 + 3, isGrass = rand()%5;
                                        if (i>0 && j>0 && i<15 && j<15
                                            && (chunk->__blockTypes[i-1][j][mZ] == Sand || 
                                                chunk->__blockTypes[i+1][j][mZ] == Sand || 
                                                chunk->__blockTypes[i][j-1][mZ] == Sand || 
                                                chunk->__blockTypes[i][j+1][mZ] == Sand))
                                            isGrass = false;
                                        if (isGrass) {
                                            for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->__blockTypes[i][j][z] = Grass;
                                        } else {
                                            for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->__blockTypes[i][j][z] = Sand;
                                        }
                                    }
                                    else if (biome[x][y] == Biome::Oasis) {
                                        int rate = rand()%5 + 3;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->__blockTypes[i][j][z] = Grass;
                                    }
                                }
                            }
                        }
                    }
                    chunk->save();
                    if (isTaller) maxHeight+=16;
                }
                if (biome[x][y]==Biome::RockyHill || biome[x][y]==Biome::MixRockyHill) color[(position.y+250)*1000+(position.x+500)] = {160, 160, 160, 255};
                else if (biome[x][y] == Biome::Oasis || biome[x][y] == Biome::MixOasis) color[(position.y+250)*1000+(position.x+500)] = {0, 204, 0, 255}; 
                else if (biome[x][y] != Biome::Sea) color[(position.y+250)*1000+(position.x+500)] = {255, 255, 153, 255};
            }
            *percent += one_part*total;
        }
    }
    void MapCreator::createTemperateZone(double* percent, const double& total, std::vector<glm::vec2>& centers, MyBase::Color* texture, const std::string& src, const glm::vec2& yBound, const float& z) {
        srand(clock());
        unsigned char count = rand()%3+2;
        int xMax = 1000, xPart = xMax/count;
        //By area tectonic
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
                xPart = xMax/(count-k);
            }
            unsigned int **map = new unsigned int*[xSize*16];
            for (int i = 0; i<xSize*16; i++) {
                map[i] = new unsigned int[1600];
                memset(map[i], 0, 6400);
            }
            glm::ivec3 origin(500-xMax-xSize, -50, z);
            glm::ivec2 bound(xSize*16, 1600);
            //Create height map and biome map
            {
                Area area(p*(rand()%10+5), {0,100}, {xSize*16, 1400});
                area.draw(bound, map);
            }
            Biome** biome = 0;
            toBiome(biome, map, bound/16);
            //Create oasis & standard biome
            {
                bool hasOasis = false;
                glm::ivec2 bound(xSize-1, 99);
                for (int x = 1; x<bound.x; x++) {
                    for (int y = 1 ;y<bound.y; y++) {
                        if (biome[x][y] == Biome::Low) {
                            if (biome[x-1][y] == Biome::Sea || biome[x+1][y] == Biome::Sea || biome[x][y-1] == Biome::Sea || biome[x][y+1] == Biome::Sea)
                                biome[x][y] = Biome::Beach;
                            else biome[x][y] = Biome::Desert;
                        }
                        else if (biome[x][y] == Biome::Mid) {
                            int rate = rand()%20;
                            if (!rate && !hasOasis) {
                                hasOasis = true;
                                int count = 1, distance = 0;
                                int mX = x-1, mY = y-1, side = 3;
                                bool noNew = false;
                                std::vector<std::pair<int,int>> store = {{x,y}};
                                while (count < 16 && !noNew) {
                                    noNew = true;
                                    for (int x = mX, y = mY; x<mX+side; x++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y] == Biome::Mid || biome[x][y] == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX, y = mY; y<mY+side; y++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y] == Biome::Mid || biome[x][y] == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX, y = mY+side-1; x<mX+side; x++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y] == Biome::Mid || biome[x][y] == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX+side-1, y = mY; y<mY+side; y++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y] == Biome::Mid || biome[x][y] == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    mX--; mY--;
                                    side += 2;
                                }
                                if (!noNew) {
                                    glm::ivec2 xBound(bound.x, 0), yBound(bound.y, 0);
                                    for (const auto& [x,y]: store) {
                                        biome[x][y] = Biome::Oasis;
                                        xBound.x = std::min(xBound.x, x); xBound.y = std::max(xBound.y, x);
                                        yBound.x = std::min(yBound.x, y); yBound.y = std::max(yBound.y, y);
                                        if  (x>0 && biome[x-1][y] != Biome::Oasis) biome[x-1][y] = Biome::MixOasis;
                                        if  (y>0 && biome[x][y-1] != Biome::Oasis) biome[x][y-1] = Biome::MixOasis;
                                        if  (x<bound.x && biome[x+1][y] != Biome::Oasis) biome[x+1][y] = Biome::MixOasis;
                                        if  (x<bound.y && biome[x][y+1] != Biome::Oasis) biome[x][y+1] = Biome::MixOasis;
                                        glm::vec2 spawn(origin.x + x, origin.y + y);
                                        centers.push_back(spawn*16.f);
                                    }
                                    glm::vec2 size( xBound.y-xBound.x, yBound.y-yBound.x);
                                    size *= 16*1.2;
                                    Tectonic lake(3, size);
                                    glm::vec2 position((xBound.y+xBound.x)/2, (yBound.y + yBound.x)/2);
                                    position *= 16;
                                    lake.setPosition(position);
                                    lake.drawDown({xSize*16, 1600}, map);
                                }
                            }
                            else {
                                if (biome[x-1][y] == Biome::High || biome[x+1][y] == Biome::High || biome[x][y-1] == Biome::High || biome[x][y+1] == Biome::High)
                                    biome[x][y] = Biome::MixRockyHill;
                                else biome[x][y] = Biome::Desert;
                            }
                        }
                        else if (biome[x][y] == Biome::High) {
                            if (biome[x-1][y] == Biome::Mid || biome[x+1][y] == Biome::Mid || biome[x][y-1] == Biome::Mid || biome[x][y+1] == Biome::Mid)
                                biome[x][y] = Biome::MixRockyHill;
                            else biome[x][y] = Biome::RockyHill;
                        }
                    }
                }
                
            }
            //Convert into real map
            glm::vec2 xBound(0, xSize), yBound(0, 50);
            std::thread threadA(createSubTemperateZone,percent, total/2/count,  biome,texture, src, bound,  map, xBound, yBound, origin);

            yBound = {50, 100};
            std::thread threadB(createSubTemperateZone,percent, total/2/count, biome, texture, src, bound,  map, xBound, yBound, origin);
            
            threadA.join();
            threadB.join();

            //Deallocate
            for (int i = 0; i<bound.x/16; i++) delete[] biome[i];
            delete[] biome;
            for (int i = 0; i<xSize; i++) delete[] map[i];
            delete[] map;
        }
    }

    void MapCreator::create(double* percent, const std::string& src) {
        MyBase::Color color[500][1000];
        for (int i =0; i<500; i++) {
            for (int j = 0; j<1000; j++) color[i][j] = BLUE;
        }
        createBedrockLayer(percent, 0.2, src, -10);
        createMagmaLayer(percent, 0.2, src, -9);
        createTopSoilLayer(percent, 0.2, src, {-8, -6});
        std::vector<glm::vec2> centers;
        createTemperateZone( percent, 0.4, centers, &color[0][0], src, {0,0}, -6);
        stbi_write_png((src+"overal.png").c_str(), 1000, 500, 4, &color[0][0], sizeof(char)*4*1000);

        glm::ivec2 spawn = centers[rand()%centers.size()];
        MyBase::File file(src+"info.bin");
        file << spawn.x << spawn.y;
        file.close();
    }
}
#include "MapCreator.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkBase.h"
#include "Color.h"
#include "File.h"
#include "Global.h"
#include "SurfaceRound.h"

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
            memset(biome[i], Biome::Sea, size.y*sizeof(Biome));
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
                if (height>72)  biome[x][y].type = Biome::High;
                else if (height>64) biome[x][y].type = Biome::Mid;
                else if (height>32) biome[x][y].type = Biome::Low;
                else biome[x][y].type = Biome::Sea;
            }
        }
    }
    void MapCreator::createOasisTree(const std::string& src, const glm::ivec3& root) {
        DynamicChunk chunk(src);
        int height = rand()%3+5;
        for (int i = 0; i<height; i++) {
            glm::ivec3 position = root;
            position.z += i;
            chunk.setType(position, OakWood);
        }
        glm::ivec3 pos[] = {{0,0,1},{1,0,0}, {-1,0,0}, {0,1,0}, {0,-1,0},
                                        {1,0,-3}, {-1,0,-3}, {0,1,-3}, {0,-1,-3}};
        for (int i = 0; i<8; i++) {
            glm::ivec3 position = pos[i] +root;
            position.z += height;
            if (!chunk.getType(position)) chunk.setType(position, OakLeaf);
        }
        for (int i = -2; i<3; i++) {
            for (int j = -2; j<3; j++) 
            if (i || j) {
                glm::ivec3 position(root.x+i, root.y+j, root.z+height-1);
                if (!chunk.getType(position)) chunk.setType(position, OakLeaf);
                position = {root.x+i, root.y+j, root.z+height-2};
                if (!chunk.getType(position)) chunk.setType(position, OakLeaf);
            }
        }
    }
    void MapCreator::createSubTropicalZone(double* percent, const double& total, Biome** biome, MyBase::Color* color, const std::string& src, const glm::vec2& bound, unsigned int** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin) {
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
                    chunk->disableList();
                    for (int i = 0; i<16; i++) {
                        for (int j = 0; j<16; j++) {
                            int mX = x*16+i, mY = y*16+j;
                            if (board[mX][mY]>maxHeight) {
                                int mZ = board[mX][mY]-maxHeight-1;
                                if (mZ>=16) isTaller = true;
                                for (int z = 0; z<=std::min(mZ, 15); z++) {
                                    if (mX==0 || mX==bound.x-1 || mX == 0 || mY ==bound.y-1) {
                                        chunk->enableLocalBit(glm::ivec3(i,j,z));
                                    }
                                    else if (board[mX-1][mY]<z+maxHeight+1 || board[mX+1][mY]<z+maxHeight+1 || board[mX][mY-1]<z+maxHeight+1 || board[mX][mY+1]<z+maxHeight+1) {
                                        chunk->enableLocalBit(glm::ivec3(i,j,z));
                                    }
                                    chunk->setLocalType(glm::ivec3(i,j,z), Stone);
                                }
                                if (mZ < 16) {
                                    chunk->enableLocalBit(glm::ivec3(i,j,mZ));
                                    if (biome[x][y].type == Biome::Desert || biome[x][y].type == Biome::Beach || biome[x][y].type == Biome::Sea) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                    }
                                    else if (biome[x][y].type == Biome::MixRockyHill && mZ < 10) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                    }
                                    else if (biome[x][y].type == Biome::MixOasis) {
                                        int rate = rand()%5 + 3, isGrass = rand()%5;
                                        if (i>0 && j>0 && i<15 && j<15
                                            && (chunk->getLocalType(glm::ivec3(i-1, j, mZ)) == Sand || 
                                                chunk->getLocalType(glm::ivec3(i+1, j, mZ)) == Sand || 
                                                chunk->getLocalType(glm::ivec3(i, j-1, mZ)) == Sand || 
                                                chunk->getLocalType(glm::ivec3(i,j+1,mZ)) == Sand))
                                            isGrass = false;
                                        if (isGrass) {
                                            for (int z = std::max(mZ - rate,0); z<mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Dirt);
                                            chunk->setLocalType(glm::ivec3(i,j,mZ), Grass);
                                        } else {
                                            for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                        }
                                    }
                                    else if (biome[x][y].type == Biome::Oasis) {
                                        int rate = rand()%5 + 3;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Dirt);
                                            chunk->setLocalType(glm::ivec3(i,j,mZ), Grass);
                                    }
                                    else if (biome[x][y].type == Biome::Lake) {
                                        int rate = rand()%5;
                                        if (board[mX][mY]+origin.z*16<biome[x][y].height-rate) {
                                            rate = rand()%5 + 2;
                                            for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                        }
                                        else {
                                            rate = rand()%5;
                                            for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Dirt);
                                            chunk->setLocalType(glm::ivec3(i,j,mZ), Grass);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    chunk->save();
                    delete chunk;
                    if (isTaller) maxHeight+=16;
                }
                if (biome[x][y].type==Biome::RockyHill || biome[x][y].type==Biome::MixRockyHill) color[(position.y+250)*1000+(position.x+500)] = {160, 160, 160, 255};
                else if (biome[x][y].type == Biome::Oasis || biome[x][y].type == Biome::MixOasis) {
                    color[(position.y+250)*1000+(position.x+500)] = {0, 204, 0, 255}; 
                    unsigned char count = rand()%10+5;
                    std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                    while (count) {
                        int i = rand()%14+1, j = rand()%14+1;
                        if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                            !matrix[i-1][j] && !matrix[i+1][j] &&
                            !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                        {
                            matrix[i][j] = 1;
                            glm::ivec3 pos = position*16 + glm::ivec3(i,j,0);
                            pos.z = origin.z*16 + board[x*16 + i][y*16+j];
                            createOasisTree(src, pos);
                            count--;
                        }
                    }
                }
                else if (biome[x][y].type == Biome::Lake) {
                    std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                    for (int i = 1; i<15; i++) {
                        for (int j = 1; j<15; j++) {
                            bool rate = rand()%16;
                            if (!rate && board[x*16+i][y*16+j]+origin.z*16>biome[x][y].height) {
                                if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                                    !matrix[i-1][j] && !matrix[i+1][j] &&
                                    !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                                {
                                    matrix[i][j] = 1;
                                    glm::ivec3 pos = position*16 + glm::ivec3(i,j,0);
                                    pos.z = origin.z*16 + board[x*16 + i][y*16+j];
                                    createOasisTree(src, pos);
                                }
                            }
                        }
                    }
                }
                else if (biome[x][y].type != Biome::Sea) color[(position.y+250)*1000+(position.x+500)] = {255, 255, 153, 255};
            }
            *percent += one_part*total;
        }
    }
    void MapCreator::createTropicalZone(double* percent, const double& total, std::vector<glm::vec2>& centers, MyBase::Color* texture, const std::string& src, const glm::vec2& yBound, const float& z) {
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
                area.applyRounds(bound, map);
            }
            Biome** biome = 0;
            toBiome(biome, map, bound/16);
            //Create oasis & standard biome
            {
                bool hasOasis = false;
                glm::ivec2 bound(xSize-1, 99);
                for (int x = 1; x<bound.x; x++) {
                    for (int y = 1 ;y<bound.y; y++) {
                        if (biome[x][y].type == Biome::Low) {
                            if (biome[x-1][y].type == Biome::Sea || biome[x+1][y].type == Biome::Sea || biome[x][y-1].type == Biome::Sea || biome[x][y+1].type == Biome::Sea)
                                biome[x][y].type = Biome::Beach;
                            else biome[x][y].type = Biome::Desert;
                        }
                        else if (biome[x][y].type == Biome::Mid) {
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
                                            if (biome[x][y].type == Biome::Mid || biome[x][y].type == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX, y = mY; y<mY+side; y++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y].type == Biome::Mid || biome[x][y].type == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX, y = mY+side-1; x<mX+side; x++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y].type == Biome::Mid || biome[x][y].type == Biome::Desert) {
                                                noNew = false;
                                                count++;
                                                store.push_back({x,y});
                                            }
                                        }
                                    }
                                    for (int x = mX+side-1, y = mY; y<mY+side; y++) {
                                        if (x>=0 && y >= 0 && x<bound.x && y<bound.y) {
                                            if (biome[x][y].type == Biome::Mid || biome[x][y].type == Biome::Desert) {
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
                                        biome[x][y].type = Biome::Oasis;
                                        xBound.x = std::min(xBound.x, x); xBound.y = std::max(xBound.y, x);
                                        yBound.x = std::min(yBound.x, y); yBound.y = std::max(yBound.y, y);
                                        if  (x>0 && biome[x-1][y].type != Biome::Oasis) biome[x-1][y].type = Biome::MixOasis;
                                        if  (y>0 && biome[x][y-1].type != Biome::Oasis) biome[x][y-1].type = Biome::MixOasis;
                                        if  (x<bound.x && biome[x+1][y].type != Biome::Oasis) biome[x+1][y].type = Biome::MixOasis;
                                        if  (x<bound.y && biome[x][y+1].type != Biome::Oasis) biome[x][y+1].type = Biome::MixOasis;
                                        glm::vec2 spawn(origin.x + x, origin.y + y);
                                        centers.push_back(spawn*16.f);
                                    }
                                    glm::vec2 size( xBound.y-xBound.x, yBound.y-yBound.x);
                                    size *= 16*1.2;
                                    {
                                        Round lake(3, size);
                                        glm::vec2 position((xBound.y+xBound.x)/2, (yBound.y + yBound.x)/2);
                                        position *= 16;
                                        lake.setPosition(position);
                                        lake.applyLake(biome, {xSize*16, 1600}, z*16, map);
                                    }
                                }
                            }
                            else {
                                if (biome[x-1][y].type == Biome::High || biome[x+1][y].type == Biome::High || biome[x][y-1].type == Biome::High || biome[x][y+1].type == Biome::High)
                                    biome[x][y].type = Biome::MixRockyHill;
                                else biome[x][y].type = Biome::Desert;
                            }
                        }
                        else if (biome[x][y].type == Biome::High) {
                            if (biome[x-1][y].type == Biome::Mid || biome[x+1][y].type == Biome::Mid || biome[x][y-1].type == Biome::Mid || biome[x][y+1].type == Biome::Mid)
                                biome[x][y].type = Biome::MixRockyHill;
                            else biome[x][y].type = Biome::RockyHill;
                        }
                    }
                }
                
            }
            //Convert into real map
            glm::vec2 xBound(0, xSize), yBound(0, 50);
            std::thread threadA(createSubTropicalZone,percent, total/2/count,  biome,texture, src, bound,  map, xBound, yBound, origin);

            yBound = {50, 100};
            std::thread threadB(createSubTropicalZone,percent, total/2/count, biome, texture, src, bound,  map, xBound, yBound, origin);
            
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
        createTropicalZone( percent, 0.4, centers, &color[0][0], src, {0,0}, -6);
        stbi_write_png((src+"overal.png").c_str(), 1000, 500, 4, &color[0][0], sizeof(char)*4*1000);

        glm::ivec2 spawn = centers[rand()%centers.size()];
        MyBase::File file(src+"info.bin");
        file << spawn.x << spawn.y;
        file.close();
    }
}
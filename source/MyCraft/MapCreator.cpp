#include "MapCreator.h"
#include "Biome.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkBase.h"
#include "Color.h"
#include "File.h"
#include "Global.h"
#include "Image.h"
#include "SurfaceRound.h"

namespace MyCraft {
    std::string MapCreator::getFileName(const std::string& src, const glm::ivec3& position) {
        size_t index = (position.x+500)*1000*1000 + (position.y+500)*1000 + (500 + position.z);
        return src + std::to_string(index)+".bin";
    }
    MapCreator::MapCreator(double* p, const std::string& src): percent(p), source(src) {}
    void MapCreator::createBedrockLayer(const double& total, const int& heightBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubBedrockLayer, percent, total/2, source, xBound, yBound, heightBound);

        xBound= {0, 500};
        std::thread threadA(createSubBedrockLayer, percent, total/2, source, xBound, yBound, heightBound);

        thread.join();
        threadA.join();
    }
    void MapCreator::createSubBedrockLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
        BlockCatogary types[4096];
        memset(&types[0], 0, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) types[i*16 + 15] = BedRock;
        unsigned int size = 0;
        BiomeManage biomeManage(src);
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                glm::ivec3 origin(x,y,height);
                std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                glm::ivec3 position = origin*16;
                biomeManage.setBiomeType(position, Biome::UnderGround);
                file.write((char*)&position, sizeof(glm::ivec3));
                file.write((char*)&size, sizeof(int));

                size = 256;
                file.write((char*)&size, sizeof(int));
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createMagmaLayer(const double& total, const int& heightBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubMagmaLayer,percent, total/2, source, xBound, yBound, heightBound);
        xBound = {0, 500};
        std::thread threadA(createSubMagmaLayer, percent, total/2, source, xBound, yBound, heightBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubMagmaLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
        BlockCatogary types[4096];
        memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) {
            types[i*16+1] = types[i*16+5] = Obsidian;
            types[i*16+2] = types[i*16+3] = types[i*16+4] = Air;
        }

        unsigned int bits[128];
        for (int i = 0; i<128; i++) bits[i] = 34<<16 | 34;

        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                glm::ivec3 origin(x,y,height);
                std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                glm::ivec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::ivec3));
                unsigned int size = 512;
                file.write((char*)&size, sizeof(int));

                file.write((char*)&bits[0], 128*sizeof(int));
                size = 3328;
                file.write((char*)&size, sizeof(int));
                file.write((char*)&types, sizeof(BlockCatogary)*4096);
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createTopSoilLayer(const double& total, const glm::ivec2& zBound) {
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubTopSoilLayer, percent, total/2, source, xBound, yBound, zBound);

        xBound = {0, 500};
        std::thread threadA(createSubTopSoilLayer, percent, total/2, source, xBound, yBound, zBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));

        unsigned int bits[128];
        for (int i = 0; i<128; i++) bits[i] = 1<<15 | 1 << 31;

        BlockCatogary types[4096];
        memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) types[i*16+15] = Sand;

        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                for (int z = zBound.x; z < zBound.y; z++) {
                    glm::ivec3 origin(x,y,z);
                    std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                    glm::ivec3 position = origin*16;
                    file.write((char*)&position, sizeof(glm::ivec3));
                    unsigned int size = 256;
                    file.write((char*)&size, sizeof(int));

                    file.write((char*)&bits[0], 128*sizeof(int));

                    size = 4096;
                    file.write((char*)&size, sizeof(int));

                    file.write((char*)&types, sizeof(BlockCatogary)*4096);
                    file.close();
                }
                *percent += one_part*total;
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
        for (int i = 0; i<9; i++) {
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
    void MapCreator::createSubTropicalZone(double* percent, const double& total, Biomes* biome, MyBase::Image* image, const std::string& src, HeightMap* map, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin) {
        double one_part = 1.0/((xBound.y-xBound.x));
        BiomeManage biomeManage(src);
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                int maxHeight = 0;
                glm::ivec3 position(x + origin.x,y + origin.y, origin.z);
                bool isTaller = true, isShow = false;
                while (isTaller) {
                    isTaller = false; isShow = false;
                    position.z = origin.z + maxHeight/16.f;
                    Chunk* chunk = Chunk::Load(src, position);
                    chunk->disableList();
                    for (int i = 0; i<16; i++) {
                        for (int j = 0; j<16; j++) {
                            int mX = x*16+i, mY = y*16+j;
                            if (map->getHeight(mX,mY)>maxHeight) {
                                int mZ = map->getHeight(mX,mY)-maxHeight-1;
                                if (mZ>=16) isTaller = true;
                                for (int z = 0; z<=std::min(mZ, 15); z++) {
                                    if (mX==0 || mX==map->getSize().x-1 || mY == 0 || mY ==map->getSize().y-1) {
                                        chunk->enableLocalBit(glm::ivec3(i,j,z));
                                    }
                                    else if (map->getHeight(mX-1,mY)<z+maxHeight+1 || map->getHeight(mX+1,mY)<z+maxHeight+1 || map->getHeight(mX,mY-1)<z+maxHeight+1 || map->getHeight(mX,mY+1)<z+maxHeight+1) {
                                        chunk->enableLocalBit(glm::ivec3(i,j,z));
                                    }
                                    chunk->setLocalType(glm::ivec3(i,j,z), Stone);
                                }
                                if (mZ < 16) {
                                    isShow = true;
                                    chunk->enableLocalBit(glm::ivec3(i,j,mZ));
                                    if (biome->getBiome(x,y).type == Biome::Desert || biome->getBiome(x,y).type == Biome::Beach || biome->getBiome(x,y).type == Biome::Sea) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                    }
                                    else if (biome->getBiome(x,y).type == Biome::MixRockyHill && mZ < 10) {
                                        int rate = rand()%5 + 2;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                            chunk->setLocalType(glm::ivec3(i,j,z), Sand);
                                    }
                                    else if (biome->getBiome(x,y).type == Biome::MixOasis) {
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
                                    else if (biome->getBiome(x,y).type == Biome::Oasis) {
                                        int rate = rand()%5 + 3;
                                        for (int z = std::max(mZ - rate,0); z<=mZ; z++)
                                                chunk->setLocalType(glm::ivec3(i,j,z), Dirt);
                                            chunk->setLocalType(glm::ivec3(i,j,mZ), Grass);
                                    }
                                    else if (biome->getBiome(x,y).type == Biome::Lake) {
                                        int rate = rand()%5;
                                        if (map->getHeight(mX,mY)+origin.z*16<biome->getBiome(x, y).height-rate) {
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
                    if (isShow) biomeManage.setBiomeType(position*16, biome->getBiome(x, y).type);
                    else biomeManage.setBiomeType(position*16, Biome::UnderGround);
                    if (isTaller) maxHeight+=16;
                }
                if (biome->getBiome(x,y).type==Biome::RockyHill || biome->getBiome(x,y).type==Biome::MixRockyHill) 
                    image->setColor(position.x+500, position.y+250, {160, 160, 160, 255});
                else if (biome->getBiome(x,y).type == Biome::Oasis || biome->getBiome(x,y).type == Biome::MixOasis) {
                    image->setColor(position.x+500, position.y+250, {0, 204, 0, 255}); 
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
                            pos.z = origin.z*16 + map->getHeight(x*16+i, y*16+j);
                            createOasisTree(src, pos);
                            count--;
                        }
                    }
                }
                else if (biome->getBiome(x,y).type == Biome::Lake) {
                    std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                    for (int i = 1; i<15; i++) {
                        for (int j = 1; j<15; j++) {
                            bool rate = rand()%16;
                            if (!rate && map->getHeight(x*16+i, y*16+j)+origin.z*16>biome->getBiome(x,y).height ) {
                                if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                                    !matrix[i-1][j] && !matrix[i+1][j] &&
                                    !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                                {
                                    matrix[i][j] = 1;
                                    glm::ivec3 pos = position*16 + glm::ivec3(i,j,0);
                                    pos.z = origin.z*16 + map->getHeight(x*16+i, y*16+j);
                                    createOasisTree(src, pos);
                                }
                            }
                        }
                    }
                }
                else if (biome->getBiome(x,y).type != Biome::Sea) image->setColor(position.x+500, position.y+250, {255, 255, 153, 255});
            }
            *percent += one_part*total;
        }
    }
    void MapCreator::createTropicalZone(const double& total, const glm::vec2& yBound, const float& z) {
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
            HeightMap map(xSize*16, 1600);
            glm::ivec3 origin(500-xMax-xSize, -50, z);
            glm::ivec2 bound(xSize*16, 1600);
            //Create height map and biome map
            {
                Area area(p*(rand()%10+5), {0,100}, {xSize*16, 1400});
                area.applyRounds(map);
            }
            Biomes biome = map;
            //Create oasis & standard biome
            {
                std::vector<std::vector<glm::ivec2>> listChunk[6];
                biome.filter(listChunk);
                for (auto& vec: listChunk[Biome::Null]) 
                    for (auto& pos: vec) biome.getBiome(pos.x, pos.y).type = Biome::Null;

                for (auto& vec: listChunk[Biome::SuperLow]) 
                    for (auto& pos: vec) biome.getBiome(pos.x, pos.y).type = Biome::Sea;

                for (auto& vec: listChunk[Biome::Low]) 
                    for (auto& pos: vec) biome.getBiome(pos.x, pos.y).type = Biome::Desert;

                for (auto& vec: listChunk[Biome::High]) 
                    for (auto& pos: vec) biome.getBiome(pos.x, pos.y).type = Biome::MixRockyHill;

                for (auto& vec: listChunk[Biome::SuperHigh]) 
                    for (auto& pos: vec) biome.getBiome(pos.x, pos.y).type = Biome::RockyHill;
                
                for (auto& vec: listChunk[Biome::Mid]) {
                    glm::imat2x2 bound(biome.getSize().x, 0, biome.getSize().y, 0);
                    for (auto& pos: vec) {
                        bound[0].x = std::min(bound[0].x, pos.x);
                        bound[0].y = std::max(bound[0].y, pos.x);
                        bound[1].x = std::min(bound[1].x, pos.y);
                        bound[1].y = std::max(bound[1].y, pos.y);
                        biome.getBiome(pos.x, pos.y).type = Biome::Oasis;
                    }
                    glm::ivec2 spawn((bound[0].y+bound[0].x)/2, (bound[1].y+bound[1].x)/2);
                    Round round(3, 16.f*glm::vec2(bound[0].y-bound[0].x, bound[1].y-bound[1].x));
                    round.setPosition(spawn*16);
                    round.applyLake(16*z, &biome, map);
                    spawn.x += origin.x;
                    spawn.y += origin.y;
                    spawn*=16;
                    spawner.push_back(spawn);
                }

            }
            //Convert into real map
            glm::vec2 xBound(0, xSize), yBound(0, 50);
            std::thread threadA(createSubTropicalZone,percent, total/2/count,  &biome, image, source, &map, xBound, yBound, origin);

            yBound = {50, 100};
            std::thread threadB(createSubTropicalZone,percent, total/2/count, &biome, image, source, &map, xBound, yBound, origin);
            
            threadA.join();
            threadB.join();
        }
    }

    void MapCreator::create(double* percent, const std::string& src) {
        MapCreator mapCreator(percent, src);
        mapCreator.image = new MyBase::Image(1000, 500, BLUE);
        mapCreator.createBedrockLayer(0.2, -10);
        mapCreator.createMagmaLayer(0.2, -9);
        mapCreator.createTopSoilLayer(0.2, {-8, -6});
        mapCreator.createTropicalZone( 0.4, {0,0}, -6);
        mapCreator.image->save(src+"overal.png");
        delete mapCreator.image;

        glm::ivec2 spawn = mapCreator.spawner[rand()%mapCreator.spawner.size()];
        MyBase::File file(src+"info.bin");
        file << spawn.x << spawn.y;
        file.close();
    }
}
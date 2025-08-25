#include "MapCreator.h"
#include "Biome.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkBase.h"
#include "Color.h"
#include "File.h"
#include "HeightMap.h"
#include "Image.h"
#include "SurfaceRound.h"
#include "ZoneCreator.h"

namespace MyCraft {
    std::string MapCreator::getFileName(const std::string& src, const glm::ivec3& position) {
        size_t index = (position.x+500)*1000*1000 + (position.y+500)*1000 + (500 + position.z);
        return src + std::to_string(index)+".bin";
    }
    MapCreator::MapCreator(double* p, const std::string& src): percent(p), source(src) {}
    void MapCreator::createBedrockLayer(const double& total, const int& heightBound) {
        glm::ivec2 xBound(- 100, 0), yBound(-250, 250);
        std::thread thread(createSubBedrockLayer, percent, total/2, source, xBound, yBound, heightBound);

        xBound= {0, 100};
        std::thread threadA(createSubBedrockLayer, percent, total/2, source, xBound, yBound, heightBound);

        thread.join();
        threadA.join();
    }
    void MapCreator::createSubBedrockLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
        char buffer[sizeof(glm::ivec3)+sizeof(int)*2 + 4096];
        memset(&buffer[0], 0, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) buffer[sizeof(glm::ivec3) + sizeof(int)*2 + i*16 + 15] = BedRock;
        unsigned int size = 256;
        memcpy(&buffer[sizeof(glm::ivec3)+sizeof(int)], (char*)&size, sizeof(int));
        BiomeManage biomeManage(src);
        for (glm::ivec3 cPosition(xBound.x,0, height); cPosition.x<xBound.y; cPosition.x++) {
            for (cPosition.y = yBound.x; cPosition.y < yBound.y; cPosition.y++) {
                std::ofstream file(getFileName(src, cPosition), std::ios::out | std::ios::binary);
                biomeManage.setBiomeType(cPosition, Biome::UnderGround);
                glm::ivec3 position = cPosition*16;
                memcpy((char*)&buffer[0], (char*)&position, sizeof(glm::ivec3));
                file.write(buffer, sizeof(glm::ivec3)+sizeof(int)*2 + 4096);
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createMagmaLayer(const double& total, const int& heightBound) {
        glm::ivec2 xBound(- 100, 0), yBound(-250, 250);
        std::thread thread(createSubMagmaLayer,percent, total/2, source, xBound, yBound, heightBound);
        xBound = {0, 100};
        std::thread threadA(createSubMagmaLayer, percent, total/2, source, xBound, yBound, heightBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubMagmaLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x));
        char buffer[sizeof(glm::ivec3) + sizeof(int)*2 + 128*4 + 4096];

        unsigned int size = 512;
        memcpy(&buffer[sizeof(glm::vec3)], (char*)&size, sizeof(int));
        unsigned int* bits = (unsigned int*)&buffer[sizeof(glm::ivec3)+sizeof(int)];
        for (int i = 0; i<128; i++) bits[i] = 34<<16 | 34;

        size = 3328;
        memcpy(&buffer[sizeof(glm::ivec3) + 128*sizeof(int) + sizeof(int)], (char*)&size, sizeof(int));
        BlockCatogary* types = (BlockCatogary*)&buffer[sizeof(glm::ivec3)+sizeof(int)*2 + 128*sizeof(int)];
        memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) {
            types[i*16+1] = types[i*16+5] = Obsidian;
            types[i*16+2] = types[i*16+3] = types[i*16+4] = Air;
        }

        for (glm::ivec3 cPosition(xBound.x, 0, height); cPosition.x<xBound.y; cPosition.x++) {
            for (cPosition.y = yBound.x; cPosition.y<yBound.y; cPosition.y++) {
                std::ofstream file(getFileName(src, cPosition), std::ios::out | std::ios::binary);
                glm::ivec3 position = cPosition*16;
                memcpy(&buffer[0], (char*)&position, sizeof(glm::ivec3));
                file.write(&buffer[0], sizeof(glm::ivec3) + sizeof(int)*2 + 128*sizeof(int)+4096*sizeof(BlockCatogary));
                file.close();
            }
            *percent += one_part*total;
        }
    }

    void MapCreator::createTopSoilLayer(const double& total, const glm::ivec2& zBound) {
        glm::ivec2 xBound(- 100, 0), yBound(-250, 250);
        std::thread thread(createSubTopSoilLayer, percent, total/2, source, xBound, yBound, zBound);

        xBound = {0, 100};
        std::thread threadA(createSubTopSoilLayer, percent, total/2, source, xBound, yBound, zBound);
        thread.join();
        threadA.join();
    }
    void MapCreator::createSubTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));

        char buffer[sizeof(glm::ivec3) + sizeof(int)*2 + 128*sizeof(int) + 4096*sizeof(BlockCatogary)];

        unsigned int size = 256;
        memcpy(&buffer[sizeof(glm::vec3)], (char*)&size, sizeof(int));
        unsigned int *bits = (unsigned int*)&buffer[sizeof(glm::ivec3) + sizeof(int)];
        for (int i = 0; i<128; i++) bits[i] = 1<<15 | 1 << 31;


        size = 4096;
        memcpy(&buffer[sizeof(glm::ivec3) + 128*sizeof(int) + sizeof(int)], (char*)&size, sizeof(int));
        BlockCatogary* types = (BlockCatogary*)&buffer[sizeof(glm::ivec3)+sizeof(int)*2 + 128*sizeof(int)];
        memset(&types[0], BlockCatogary::Stone, sizeof(BlockCatogary)*4096);
        for (int i = 0; i<256; i++) types[i*16+15] = Sand;

        for (glm::ivec3 cPosition(xBound.x, 0, 0); cPosition.x<xBound.y; cPosition.x++) {
            for (cPosition.y = yBound.x; cPosition.y<yBound.y; cPosition.y++) {
                for (cPosition.z = zBound.x; cPosition.z < zBound.y; cPosition.z++) {
                    std::ofstream file(getFileName(src, cPosition), std::ios::out | std::ios::binary);
                    glm::ivec3 position = cPosition*16;
                    memcpy(&buffer[0], (char*)&position, sizeof(glm::ivec3));
                    file.write(&buffer[0], sizeof(glm::ivec3) + sizeof(int)*2 + 128*sizeof(int) + 4096*sizeof(BlockCatogary));
                    file.close();
                }
                *percent += one_part*total;
            }
        }
    }
    void MapCreator::createSubZone(const Zone* zone, double* percent, const double& total, Biomes* biome, MyBase::Image* image, const std::string& src, HeightMap* map, const glm::vec2& xBound, const glm::vec2& yBound, const int& z) {
        double one_part = 1.0/((xBound.y-xBound.x));
        BiomeManage biomeManage(src);
        for (glm::ivec3 cPosition(xBound.x,0, z); cPosition.x<xBound.y; cPosition.x++) {
            for (cPosition.y = yBound.x; cPosition.y<yBound.y; cPosition.y++) {
                int maxX = cPosition.x*16+16, maxY = cPosition.y*16+16;
                int maxHeight = 16*z-1;
                bool isTaller = true, isShow = false;
                while (isTaller) {
                    isTaller = false;
                    cPosition.z = (maxHeight+1)/16;
                    Chunk* chunk = Chunk::Load(0, src, cPosition);
                    chunk->disableList();
                    for (glm::ivec3 position(cPosition*16); position.x < maxX; position.x++) {
                        for (position.y = cPosition.y*16; position.y< maxY; position.y++) {
                            if ((*map)[position]>maxHeight) {
                                int mZ = (*map)[position]-maxHeight;
                                if (mZ>16) isTaller = true;
                                int maxZ = maxHeight + std::min(mZ, 16);
                                for (position.z=maxHeight+1; position.z<=maxZ; position.z++) {
                                    if (map->isShow(position)) chunk->enableBit(position);
                                    chunk->setType(position, Stone);
                                }
                                if (mZ <= 16) {
                                    isShow = true;
                                    position.z--;
                                    zone->FillColumn(*chunk, *map, (*biome)[cPosition], position);
                                }
                            }
                        }
                    }
                    chunk->save();
                    delete chunk;
                    if (isShow) biomeManage.setBiomeType(cPosition, (*biome)[cPosition].type);
                    else if (isTaller) biomeManage.setBiomeType(cPosition, Biome::UnderGround);
                    else biomeManage.setBiomeType(cPosition, (*biome)[cPosition].type);
                    if (isTaller) maxHeight+=16;
                }
                zone->FillChunk(src, *image, *map, cPosition, (*biome)[cPosition]);
            }
            *percent += one_part*total;
        }
    }
    void MapCreator::createZone(const Zone& zone, const double& total, const glm::vec2& yBound, const int& z) {
        srand(clock());
        unsigned char count = rand()%2+1;
        int xMax = 200, xPart = xMax/count;
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
            HeightMap map(xSize*16, (yBound.y-yBound.x)*16, z*16-1);
            glm::ivec3 origin(100-xMax-xSize, yBound.x, z);
            map.setPosition(origin*16);
            //Create height map and biome map
            {
                Area area(p*zone.getTectonicPerArea(), glm::ivec2(origin.x*16+50,origin.y*16 + 50), map.getSize() - glm::ivec2(200, 200));
                area.applyRounds(map);
            }
            Biomes biome = map;
            //Create oasis & standard biome
            zone.AnalysSurface(spawner, map, biome);
            //Convert into real map
            int midY = (yBound.x+yBound.y)/2;
            glm::vec2 xBound(origin.x, origin.x + xSize);
            std::thread threadA(createSubZone, &zone, percent, total/2/count,  &biome, image, source, &map, xBound, glm::ivec2(yBound.x, midY), z);

            std::thread threadB(createSubZone, &zone, percent, total/2/count, &biome, image, source, &map, xBound, glm::ivec2(midY, yBound.y), z);
            
            threadA.join();
            threadB.join();
        }
    }

    void MapCreator::create(double* percent, const std::string& src) {
        MapCreator mapCreator(percent, src);
        mapCreator.image = new MyBase::Image(300, 500, BLUE);
        mapCreator.image->setPosition({-150, -250});
        mapCreator.createBedrockLayer(0.1, -8);
        mapCreator.createTopSoilLayer(0.1, {-7, -6});
        mapCreator.createZone(Temperate(), 0.4, {-250,0}, -6);
        mapCreator.createZone(Tropical(), 0.4, {0,250}, -6);
        mapCreator.image->save(src+"overal.png");
        delete mapCreator.image;

        if (mapCreator.spawner.size()) {
            glm::ivec3 spawn = mapCreator.spawner[rand()%mapCreator.spawner.size()];
            MyBase::File file(src+"info.bin");
            file << spawn.x << spawn.y << spawn.z;
            file.close();
        }
        *percent = 1;
    }
}
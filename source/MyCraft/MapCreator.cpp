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

namespace MyCraft {
    void Tropical::AnalysSurface(std::vector<glm::ivec2>& spawner, HeightMap& map, Biomes& biome) const {
        std::vector<glm::ivec2> listChunk;
        biome.filter(listChunk, Biome::Mid);
        
        glm::imat2x2 bound(biome.getPosition().x + biome.getSize().x, biome.getPosition().x, biome.getPosition().y + biome.getSize().y, biome.getPosition().y);
        for (auto& pos: listChunk) {
            bound[0].x = std::min(bound[0].x, pos.x);
            bound[0].y = std::max(bound[0].y, pos.x);
            bound[1].x = std::min(bound[1].x, pos.y);
            bound[1].y = std::max(bound[1].y, pos.y);
            biome[pos].type = Biome::Oasis;
            glm::ivec2 cPosition = pos;
            cPosition.x--;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.y++;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.x++;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.x++;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.y--;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.y--;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.x--;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
            cPosition.x--;
            if (biome.isValid(cPosition)) biome[cPosition].type = Biome::MixOasis;
        }
        glm::ivec2 spawn((bound[0].y+bound[0].x)/2, (bound[1].y+bound[1].x)/2);
        Round round(3, 16.f*0.8f*glm::vec2(bound[0].y-bound[0].x, bound[1].y-bound[1].x));
        round.setPosition(spawn*16);
        round.applyLake(&biome, map);
        spawn*=16;
        spawner.push_back(spawn);
        int maxX = biome.getPosition().x + biome.getSize().x, maxY = biome.getPosition().y + biome.getSize().y;
        for (glm::ivec2 cPosition(biome.getPosition().x, 0); cPosition.x < maxX; cPosition.x++) {
            for (cPosition.y=biome.getPosition().y; cPosition.y<maxY; cPosition.y++) {
                switch (biome[cPosition].type) {
                    case Biome::SuperLow: biome[cPosition].type = Biome::Sea; break;
                    case Biome::Low: biome[cPosition].type = Biome::Desert; break;
                    case Biome::Mid: biome[cPosition].type = Biome::Desert; break;
                    case Biome::High: biome[cPosition].type = Biome::MixRockyHill; break;
                    case Biome::SuperHigh: biome[cPosition].type = Biome::RockyHill; break;
                    default: break;
                }
            }
        }
    }
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
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubMagmaLayer,percent, total/2, source, xBound, yBound, heightBound);
        xBound = {0, 500};
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
        glm::ivec2 xBound(- 500, 0), yBound(-250, 250);
        std::thread thread(createSubTopSoilLayer, percent, total/2, source, xBound, yBound, zBound);

        xBound = {0, 500};
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
    void MapCreator::createOasisTree(DynamicChunk& chunk, const glm::ivec3& root) {
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
    void MapCreator::FillColumn(Chunk& chunk, HeightMap& map, Biome& biome, const glm::ivec3& ceiling) {
        switch (biome.type) {
            case Biome::Desert: {
                int mod = ceiling.z%16;
                if (mod<0) mod = 16 - (-ceiling.z)%16;
                int rate = std::min(rand()%5 + 3, mod);
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                    chunk.setType(position, Sand);
            }
                break;
            case Biome::Beach:{
                int mod = ceiling.z%16;
                if (mod<0) mod = 16 - (-ceiling.z)%16;
                int rate = std::min(rand()%5 + 3, mod);
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                    chunk.setType(position, Sand);
            }
                break;
            case Biome::Lake: {
                int rate = rand()%5;
                if (map[ceiling]<biome.height-rate) {
                    int mod = ceiling.z%16;
                    if (mod<0) mod = 16 - (-ceiling.z)%16;
                    int rate = std::min(rand()%5 + 3, mod);
                    for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                            chunk.setType(position, Sand);
                }
                else {
                    int mod = ceiling.z%16;
                    if (mod<0) mod = 16 - (-ceiling.z)%16;
                    int rate = std::min(rand()%5, mod);
                    for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                        chunk.setType(position, Dirt);
                    chunk.setType(ceiling, Grass);
                }
            }
                break;
            case Biome::RockyHill:
                break;
            case Biome::MixRockyHill: {
                if (ceiling.z < 10) {
                    int mod = ceiling.z%16;
                    if (mod<0) mod = 16 - (-ceiling.z)%16;
                    int rate = std::min(rand()%5 + 3, mod);
                    for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                        chunk.setType(position, Sand);
                }
            }
                break;
            case Biome::MixOasis: {
                int mod = ceiling.z%16;
                if (mod<0) mod = 16 - (-ceiling.z)%16;
                int rate = std::min(rand()%5 + 3, mod), isGrass = rand()%5;
                if (ceiling.x>0 && ceiling.y>0 && ceiling.x<15 && ceiling.y<15
                    && (chunk.getLocalType(glm::ivec3(ceiling.x-1, ceiling.y, ceiling.z)) == Sand || 
                        chunk.getLocalType(glm::ivec3(ceiling.x+1, ceiling.y, ceiling.z)) == Sand || 
                        chunk.getLocalType(glm::ivec3(ceiling.x, ceiling.y-1, ceiling.z)) == Sand || 
                        chunk.getLocalType(glm::ivec3(ceiling.x,ceiling.y+1, ceiling.z)) == Sand))
                    isGrass = false;
                if (isGrass) {
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                        chunk.setType(position, Dirt);
                    chunk.setType(ceiling, Grass);
                } else {
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                        chunk.setType(ceiling, Sand);
                }
            }
                break;
            case Biome::Oasis: {
                int mod = ceiling.z%16;
                if (mod<0) mod = 16 - (-ceiling.z)%16;
                int rate = std::min(rand()%5 + 3, mod);
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                    chunk.setType(position, Dirt);
                chunk.setType(ceiling, Grass);
            }
                break;
            case Biome::Sea:{
                int mod = ceiling.z%16;
                if (mod<0) mod = 16 - (-ceiling.z)%16;
                int rate = std::min(rand()%5 + 3, mod); 
                for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                    chunk.setType(position, Sand);
            }
                break;
            default:
                break;
        }
    }

    void MapCreator::FillChunk(const std::string& src, MyBase::Image& image, HeightMap& map, const glm::ivec3& origin, const Biome& biome) {
        switch (biome.type) {
            case Biome::Desert: {
                image.setColor(origin.x, origin.y, {255, 255, 153, 255});
            }
                break;
            case Biome::Beach:{
                image.setColor(origin.x, origin.y, {255, 255, 153, 255});
            }
                break;
            case Biome::Lake: {
                std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                DynamicChunk chunk(src);
                for (int i = 1; i<15; i++) {
                    for (int j = 1; j<15; j++) {
                        bool rate = rand()%16;
                        glm::ivec3 position(origin.x*16+i, origin.y*16+j, origin.z*16);
                        
                        if (!rate && map[position]>biome.height ) {
                            if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                                !matrix[i-1][j] && !matrix[i+1][j] &&
                                !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                            {
                                matrix[i][j] = 1;
                                position.z += map[position];
                                createOasisTree(chunk, position);
                            }
                        }
                    }
                }
            }
                break;
            case Biome::RockyHill: {
                image.setColor(origin.x, origin.y, {160, 160, 160, 255});
            }
                break;
            case Biome::MixRockyHill: {
                image.setColor(origin.x, origin.y, {160, 160, 160, 255});
            }
                break;
            case Biome::MixOasis: {
                image.setColor(origin.x, origin.y, {0, 204, 0, 255}); 
                unsigned char count = rand()%10+5;
                std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                DynamicChunk chunk(src);
                while (count) {
                    int i = rand()%14+1, j = rand()%14+1;
                    if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                        !matrix[i-1][j] && !matrix[i+1][j] &&
                        !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                    {
                        matrix[i][j] = 1;
                        glm::ivec3 position = origin*16 + glm::ivec3(i,j,0);
                        position.z = map[position];
                        createOasisTree(chunk, position);
                        count--;
                    }
                }
            }
                break;
            case Biome::Oasis: {
                image.setColor(origin.x, origin.y, {0, 204, 0, 255}); 
                unsigned char count = rand()%10+5;
                std::vector<std::vector<bool>> matrix(16, std::vector<bool>(16, 0));
                DynamicChunk chunk(src);
                while (count) {
                    int i = rand()%14+1, j = rand()%14+1;
                    if (!matrix[i-1][j-1] && !matrix[i+1][j-1] && !matrix[i][j-1] && 
                        !matrix[i-1][j] && !matrix[i+1][j] &&
                        !matrix[i-1][j+1] && !matrix[i+1][j] && !matrix[i+1][j+1])
                    {
                        matrix[i][j] = 1;
                        glm::ivec3 position = origin*16 + glm::ivec3(i,j,0);
                        position.z = map[position];
                        createOasisTree(chunk, position);
                        count--;
                    }
                }
            }
                break;
            case Biome::Sea:{
            }
                break;
            case Biome::Null: {
                if (origin.x>image.getPosition().x && origin.x<image.getPosition().x + image.getSize().x - 1 && origin.y>image.getPosition().y && origin.y<image.getPosition().y+image.getSize().y - 1) {
                    int r = 0, g = 0, b = 0;
                    r = image.getColor(origin.x-1, origin.y).red + image.getColor(origin.x+1, origin.y).red + image.getColor(origin.x, origin.y-1).red + image.getColor(origin.x, origin.y+1).red;
                    b = image.getColor(origin.x-1, origin.y).blue + image.getColor(origin.x+1, origin.y).blue + image.getColor(origin.x, origin.y-1).blue + image.getColor(origin.x, origin.y+1).blue;
                    g = image.getColor(origin.x-1, origin.y).green + image.getColor(origin.x+1, origin.y).green + image.getColor(origin.x, origin.y-1).green + image.getColor(origin.x, origin.y+1).green;
                    MyBase::Color color;
                    color.red = r/4;
                    color.blue = b/4;
                    color.green = g/4;
                    color.alpha = 255;
                    image.setColor(origin.x, origin.y, color);
                }
            }
                break;
            default:
                break;
        }
    }
    void MapCreator::createSubZone(double* percent, const double& total, Biomes* biome, MyBase::Image* image, const std::string& src, HeightMap* map, const glm::vec2& xBound, const glm::vec2& yBound, const int& z) {
        double one_part = 1.0/((xBound.y-xBound.x));
        BiomeManage biomeManage(src);
        for (glm::ivec3 cPosition(xBound.x,0, z); cPosition.x<xBound.y; cPosition.x++) {
            for (cPosition.y = yBound.x; cPosition.y<yBound.y; cPosition.y++) {
                    int maxX = cPosition.x*16+16, maxY = cPosition.y*16+16;
                int maxHeight = 16*z-1;
                bool isTaller = true, isShow = false;
                while (isTaller) {
                    isTaller = false; isShow = false;
                    cPosition.z = (maxHeight+1)/16.f;
                    Chunk* chunk = Chunk::Load(src, cPosition);
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
                                    FillColumn(*chunk, *map, (*biome)[cPosition], position);
                                }
                            }
                        }
                    }
                    chunk->save();
                    delete chunk;
                    if (isShow) biomeManage.setBiomeType(cPosition, (*biome)[cPosition].type);
                    else biomeManage.setBiomeType(cPosition, Biome::UnderGround);
                    if (isTaller) maxHeight+=16;
                }
                FillChunk(src, *image, *map, cPosition, (*biome)[cPosition]);
            }
            *percent += one_part*total;
        }
    }
    void MapCreator::createZone(const Zone& zone, const double& total, const glm::vec2& yBound, const int& z) {
        srand(clock());
        unsigned char count = rand()%5+3;
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
            HeightMap map(xSize*16, (yBound.y-yBound.x)*16, z*16-1);
            glm::ivec3 origin(500-xMax-xSize, yBound.x, z);
            map.setPosition(origin*16);
            //Create height map and biome map
            {
                Area area(p*(rand()%10+5), glm::ivec2(origin.x*16+100,origin.y*16 + 100), map.getSize() - glm::ivec2(200, 200));
                area.applyRounds(map);
            }
            Biomes biome = map;
            //Create oasis & standard biome
            zone.AnalysSurface(spawner, map, biome);
            //Convert into real map
            int midY = (yBound.x+yBound.y)/2;
            glm::vec2 xBound(origin.x, origin.x + xSize);
            std::thread threadA(createSubZone,percent, total/2/count,  &biome, image, source, &map, xBound, glm::ivec2(yBound.x, midY), z);

            std::thread threadB(createSubZone,percent, total/2/count, &biome, image, source, &map, xBound, glm::ivec2(midY, yBound.y), z);
            
            threadA.join();
            threadB.join();
        }
    }

    void MapCreator::create(double* percent, const std::string& src) {
        MapCreator mapCreator(percent, src);
        mapCreator.image = new MyBase::Image(1000, 500, BLUE);
        mapCreator.image->setPosition({-500, -250});
        mapCreator.createBedrockLayer(0.2, -10);
        mapCreator.createMagmaLayer(0.2, -9);
        mapCreator.createTopSoilLayer(0.2, {-8, -6});
        mapCreator.createZone(Tropical(), 0.4, {-50,50}, -6);
        mapCreator.image->save(src+"overal.png");
        delete mapCreator.image;

        glm::ivec2 spawn = mapCreator.spawner[rand()%mapCreator.spawner.size()];
        MyBase::File file(src+"info.bin");
        file << spawn.x << spawn.y;
        file.close();
    }
}
#include "ZoneCreator.h"
#include "SurfaceRound.h"
namespace MyCraft {
    int Tropical::getTectonicPerArea() const {
        return (rand()%10+5);
    }
    void Tropical::AnalysSurface(std::vector<glm::ivec3>& spawner, HeightMap& map, Biomes& biome) const {
        std::vector<glm::ivec2> listChunk;
        biome.filter(listChunk, Biome::Mid);
        if (listChunk.size()) {
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
            glm::ivec3 spawn((bound[0].y+bound[0].x)/2, (bound[1].y+bound[1].x)/2, 0);
            Round round(3, 16.f*0.8f*glm::vec2(bound[0].y-bound[0].x, bound[1].y-bound[1].x));
            round.setPosition(spawn*16);
            round.applyLake(&biome, map);
            spawn*=16;
            spawn.z = map[spawn];
            spawner.push_back(spawn);
        }
        int maxX = biome.getPosition().x + biome.getSize().x, maxY = biome.getPosition().y + biome.getSize().y;
        for (glm::ivec2 cPosition(biome.getPosition().x, 0); cPosition.x < maxX; cPosition.x++) {
            for (cPosition.y=biome.getPosition().y; cPosition.y<maxY; cPosition.y++) {
                if (biome[cPosition].type>=Biome::SuperLow && biome[cPosition].type<=Biome::SuperHigh)
                    biome[cPosition].height = biome.getAverageHeight(biome[cPosition].type);
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

    void Tropical::FillColumn(Chunk& chunk, HeightMap& map, Biome& biome, const glm::ivec3& ceiling) const {
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
                
                int height = std::min(biome.height, chunk.getPosition().z+16);
                for (glm::ivec3 position(ceiling.x, ceiling.y, ceiling.z+1); position.z<height; position.z++) {
                    chunk.setType(position, Water);
                    chunk.pourWater(position, glm::vec4(1));
                }
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
                    
                    int height = std::min(biome.height, chunk.getPosition().z+16);
                    for (glm::ivec3 position(ceiling.x, ceiling.y, ceiling.z+1); position.z<height; position.z++) {
                        chunk.setType(position, Water);
                        chunk.pourWater(position, glm::vec4(1));
                    }
                }
                else {
                    int mod = ceiling.z%16;
                    if (mod<0) mod = 16 - (-ceiling.z)%16;
                    int rate = std::min(rand()%5, mod);
                    for (glm::ivec3 position(ceiling.x,ceiling.y, ceiling.z-rate); position.z<=ceiling.z; position.z++)
                        chunk.setType(position, Dirt);
                    chunk.setType(ceiling, Grass);

                    int height = std::min(biome.height, chunk.getPosition().z+16);
                    for (glm::ivec3 position(ceiling.x, ceiling.y, ceiling.z+1); position.z<height; position.z++) {
                        chunk.setType(position, Water);
                        chunk.pourWater(position, glm::vec4(1));
                    }
                }
            }
                break;
            case Biome::RockyHill:
                break;
            case Biome::MixRockyHill: {
                if (ceiling.z <= biome.height) {
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

    void Tropical::FillChunk(const std::string& src, MyBase::Image& image, HeightMap& map, const glm::ivec3& origin, const Biome& biome) const {
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

    void Tropical::createOasisTree(DynamicChunk& chunk, const glm::ivec3& root) const {
        int height = rand()%3+5;
        for (int i = 0; i<height; i++) {
            glm::ivec3 position = root;
            position.z += i;
            chunk.setType(position, OakLog);
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
}
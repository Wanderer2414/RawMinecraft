#include "ChunkLoader.h"
#include "Block.h"
#include <cstring>

namespace MyCraft {
    ChunkLoader::ChunkLoader() {

    }
    ChunkLoader::~ChunkLoader() {

    }
    const std::vector<glm::vec4>& ChunkLoader::getChunks() const {
        return __chunkPositions;
    }
    std::string ChunkLoader::getFileName(const glm::vec3& position) const {
        size_t index = (position.x+500)*1000*100+(position.y+500)*100+(position.z+50);
        return "bin/"+std::to_string(index)+".bin";
    }
    void ChunkLoader::create() const {
        for (int i = -10; i<10; i++) {
            for (int j = -10; j<10; j++) {
                for (int k = -5; k<5; k++) {
                    size_t index = (i+500)*1000*100+(j+500)*100+(k+50);
                    index = 100*index/(1000*1000*100);
                    std::cout << index << std::endl;
                    glm::vec3 position = 16.0f*glm::vec3(i,j,k);
                    std::ofstream file(getFileName(position), std::ios::out | std::ios::binary);
                    unsigned int size = 0;
                    if (k<-1) {
                        file.write((char*)&size, sizeof(int));
                        BlockCatogary::Catogary types[256];
                        memset(types, 1, sizeof(BlockCatogary::Catogary)*256);
                        file.write((char*)&types, sizeof(BlockCatogary::Catogary)*256);
                    }
                    else if (k==-1) {
                        size = 1;
                        file.write((char*)&size, sizeof(int));

                        BlockCatogary::Catogary type = BlockCatogary::Grass;
                        file.write((char*)&type, sizeof(BlockCatogary::Catogary));
                        size = 256;
                        file.write((char*)&size, sizeof(int));

                        for (int x = 0; x<16; x++) {
                            for (int y = 0; y<16; y++) {
                                glm::vec4 pos = glm::vec4(position+glm::vec3(x,y,15),1);
                                file.write((char*)&pos, sizeof(glm::vec4));
                            }
                        }

                        BlockCatogary::Catogary types[256];
                        memset(types, 1, sizeof(BlockCatogary::Catogary)*256);
                        file.write((char*)&types, sizeof(BlockCatogary::Catogary)*256);
                    }
                    else {
                        file.write((char*)&size, sizeof(int));
                        BlockCatogary::Catogary types[256];
                        memset(types, 0, sizeof(BlockCatogary::Catogary)*256);
                        file.write((char*)&types, sizeof(BlockCatogary::Catogary)*256);
                    }
                    file.close();
                }
            }
        }
    }
    void ChunkLoader::load(const glm::vec3& position) {

    }

    Chunk& ChunkLoader::operator[](const glm::vec3& pos) {
        glm::vec3 position(floor(pos.x-__position.x)/16, floor(pos.y-__position.y)/16, floor(pos.z-__position.z)/16);
        return *__chunks[__chunkIndices[(int)position.x][(int)position.y][(int)position.z]];
    }
    const Chunk& ChunkLoader::operator[](const glm::vec3& pos) const {
        glm::vec3 position(floor(pos.x-__position.x)/16, floor(pos.y-__position.y)/16, floor(pos.z-__position.z)/16);
        return *__chunks[__chunkIndices[(int)position.x][(int)position.y][(int)position.z]];
    }
    void ChunkLoader::glDraw() const {
        
    }

}
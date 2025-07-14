#include "ChunkLoader.h"
#include "Block.h"
namespace MyCraft {
    ChunkLoader::ChunkLoader(): __position(0, 0, -1) {
        __chunks.resize(125);
        __chunkPositions.resize(125);
        load(glm::vec3(0,0,0));
    }
    ChunkLoader::~ChunkLoader() {
        for (auto& chunk:__chunks) delete chunk;
    }
    const std::vector<glm::vec4>& ChunkLoader::getChunks() const {
        return __chunkPositions;
    }
    std::string ChunkLoader::getFileName(const glm::vec3& position) const {
        size_t index = (floor(position.x)+500)*1000*100 + (floor(position.y)+500)*100 + (50 + floor(position.z));
        return "bin/"+std::to_string(index)+".bin";
    }
    void ChunkLoader::create() const {
        for (int i = -10; i<10; i++) {
            for (int j = -10; j<10; j++) {
                for (int k = -5; k<5; k++) {
                    size_t index = (i+500)*1000*100+(j+500)*100+(k+50);
                    index = 100*index/(1000*1000*100);
                    std::cout << index << std::endl;
                    glm::vec3 position = glm::vec3(i,j,k);
                    std::ofstream file(getFileName(position), std::ios::out | std::ios::binary);
                    position *= 16;
                    file.write((char*)&position, sizeof(glm::vec3));
                    unsigned int size = 0;
                    if (k<-1) {
                        file.write((char*)&size, sizeof(int));
                        BlockCatogary::Catogary types[256];
                        memset(types, 1, sizeof(BlockCatogary::Catogary)*256);
                        for (int z = 0; z<16; z++)
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

                        BlockCatogary::Catogary types[16];

                        memset(types, 0, sizeof(BlockCatogary::Catogary)*16);
                        types[15] = BlockCatogary::Grass;
                        for (int z = 0; z<256; z++)
                            file.write((char*)&types, sizeof(BlockCatogary::Catogary)*16);
                    }
                    else {
                        file.write((char*)&size, sizeof(int));
                        BlockCatogary::Catogary types[256];
                        memset(types, 0, sizeof(BlockCatogary::Catogary)*256);
                        for (int z = 0; z<16; z++)
                            file.write((char*)&types, sizeof(BlockCatogary::Catogary)*256);
                        
                    }
                    file.close();
                }
            }
        }
    }
    void ChunkLoader::load(const glm::vec3& pos) {
        int x = floor(pos.x/16), y = floor(pos.y/16), z= floor(pos.z/16);
        if (x != floor(__position.x)/16 || y != floor(__position.y)/16  || y != floor(__position.y)/16 ) {
            for (auto& chunk: __chunks) delete chunk;
            clear();
            for (int i = 0; i<5; i++) {
                for (int j = 0; j<5; j++) {
                    for (int k = 0; k<5; k++) {
                        int index = i*25+j*5+k;
                        __chunks[index] = Chunk::Load(getFileName(glm::vec3(x+i,y+j,z+k)));
                        __chunkIndices[i][j][k] = index;
                        __chunkPositions[index] = glm::vec4(16.f*glm::vec3(x+i,y+j,z+k),1);
                        insert(__chunks[index]);
                    }
                }
            }
            __position = glm::vec3(x,y,z)*16.f;
        }
    }

    Chunk& ChunkLoader::operator[](const glm::vec3& pos) {
        int x = floor((pos.x-__position.x)/16), y = floor((pos.y-__position.y)/16), z= floor((pos.z-__position.z)/16);
        
        return *__chunks[__chunkIndices[x][y][z]];
    }
    const Chunk& ChunkLoader::operator[](const glm::vec3& pos) const {
        int x = floor((pos.x-__position.x)/16), y = floor((pos.y-__position.y)/16), z= floor((pos.z-__position.z)/16);
        
        return *__chunks[__chunkIndices[x][y][z]];
    }
    void ChunkLoader::glDraw() const {
        Container3D::glDraw();
    }

}
#include "ChunkLoader.h"
#include "Block.h"
namespace MyCraft {
    ChunkLoader::ChunkLoader(): __isLoaded(false) {
        __chunks.resize(world_side*world_side*world_side, 0);
        __chunkPositions.resize(world_side*world_side*world_side);
        for (int i = 0; i<world_side; i++)
            for (int j = 0; j<world_side; j++) 
                for (int k = 0; k<world_side; k++) 
                    __chunkIndices[i][j][k] = i*world_side*world_side + j*world_side + k;
        playerAt({0,0,0});
    }
    ChunkLoader::~ChunkLoader() {
        for (auto& chunk:__chunks) delete chunk;
    }
    const std::vector<glm::vec4>& ChunkLoader::getChunks() const {
        return __chunkPositions;
    }
    std::string ChunkLoader::getFileName(const glm::ivec3& position) const {
        size_t index = (position.x+500)*1000*100 + (position.y+500)*100 + (50 + position.z);
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
    void ChunkLoader::playerAt(const glm::vec3& pos) {
        glm::ivec3 position(floor(pos.x/16)-floor(world_side/2.f), floor(pos.y/16)-floor(world_side/2.f),  floor(pos.z/16) - floor(world_side/2.f));
        if (__isLoaded) {
            if (position != __position) {
                clear();
                int sub_buffer[world_side][world_side][world_side];
                memcpy(sub_buffer, __chunkIndices, world_side*world_side*world_side*sizeof(int));
                glm::ivec3 delta = position - __position;
                for (int i = 0; i<world_side; i++) {
                    for (int j = 0; j<world_side; j++) {
                        for (int k = 0; k<world_side; k++) {
                            glm::ivec3 old_pos = glm::ivec3(i,j,k)+delta;
                            if (old_pos.x < 0 || old_pos.x >= world_side || 
                                old_pos.y < 0 || old_pos.y >= world_side || 
                                old_pos.z < 0 || old_pos.z >= world_side) 
                            {
                                old_pos.x = (old_pos.x+world_side)%world_side;
                                old_pos.y = (old_pos.y+world_side)%world_side;
                                old_pos.z = (old_pos.z+world_side)%world_side;
                                __chunkIndices[i][j][k] = sub_buffer[old_pos.x][old_pos.y][old_pos.z];
                                __chunks[__chunkIndices[i][j][k]]->save();
                                delete __chunks[__chunkIndices[i][j][k]];
                                glm::ivec3 origin = position+glm::ivec3(i,j,k);
                                __chunks[__chunkIndices[i][j][k]] = Chunk::Load(getFileName(origin));
                                __chunkPositions[__chunkIndices[i][j][k]] = glm::vec4(origin*16, 1);
                            }
                            else {
                                __chunkIndices[i][j][k] = sub_buffer[old_pos.x][old_pos.y][old_pos.z];
                            }
                            insert(__chunks[__chunkIndices[i][j][k]]);
                        }
                    }
                }
                __position = position;
            }
        }
        else {
            __isLoaded = true;
            __position = position;
            for (int i = 0; i<world_side; i++) {
                for (int j = 0; j<world_side; j++) {
                    for (int k = 0; k<world_side; k++) {
                        glm::ivec3 origin = __position + glm::ivec3(i,j,k);
                        __chunks[__chunkIndices[i][j][k]] = Chunk::Load(getFileName(origin));
                        __chunkPositions[__chunkIndices[i][j][k]] = glm::vec4(origin*16, 1);
                        insert(__chunks[__chunkIndices[i][j][k]]);
                    }
                }
            }
        }
    }

    Chunk& ChunkLoader::operator[](const glm::vec3& pos) {
        int x = floor(pos.x/16-__position.x), y = floor(pos.y/16-__position.y), z= floor(pos.z/16-__position.z);
        return *__chunks[__chunkIndices[x][y][z]];
    }
    const Chunk& ChunkLoader::operator[](const glm::vec3& pos) const {
        int x = floor(pos.x/16 -__position.x), y = floor(pos.y/16 - __position.y), z= floor(pos.z/16-__position.z);
        return *__chunks[__chunkIndices[x][y][z]];
    }
    void ChunkLoader::glDraw() const {
        Container3D::glDraw();
    }

}
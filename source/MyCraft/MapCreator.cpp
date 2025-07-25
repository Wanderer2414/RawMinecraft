#include "MapCreator.h"
#include "Block.h"
#include "Global.h"
namespace MyCraft {
    std::string MapCreator::getFileName(const std::string& src, const glm::ivec3& position) {
        size_t index = (position.x+500)*1000*1000 + (position.y+500)*1000 + (500 + position.z);
        return src + std::to_string(index)+".bin";
    }
    void MapCreator::createPlaneSurface(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound) {
        std::thread* threads[100];
        memset(&threads[0], 0, sizeof(void*)*100);
        int index = 0;
        for (int i = 4; i<6; i++) {
            for (int j = 4; j<6; j++) {
                glm::ivec2 xBound(i*100 - 500, (i+1)*100 -500), yBound(j*100-500, (j+1)*100 - 500);
                threads[index++] = new std::thread(createSubPlaneSurface, mtx, percent, total/4, src, xBound, yBound, heightBound);
            }
        }
        for (int i = 0; i<100; i++) {
            if (threads[i]) {
                threads[i]->join();
                delete threads[i];
            }
        }
    }
    void MapCreator::createSubPlaneSurface(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height) {
        double one_part = 1.0/((xBound.y-xBound.x)*(yBound.y-yBound.x));
        int z = std::floor(1.0f*height/16), offset = height - z*16;
        for (int x = xBound.x; x<xBound.y; x++) {
            for (int y = yBound.x; y<yBound.y; y++) {
                glm::ivec3 origin(x,y,z);
                std::ofstream file(getFileName(src, origin), std::ios::out | std::ios::binary);
                glm::vec3 position = origin*16;
                file.write((char*)&position, sizeof(glm::vec3));
                unsigned int size = 1;
                file.write((char*)&size, sizeof(int));
                BlockCatogary::Catogary type = BlockCatogary::Catogary::Grass;
                file.write((char*)&type, sizeof(BlockCatogary::Catogary));
                size = 256;
                file.write((char*)&size, sizeof(int));
                unsigned int bits[128];
                for (int i = 0; i<128; i++) bits[i] = 1<<31 | 1<<15;
                file.write((char*)&bits[0], 128*sizeof(int));
                file.write((char*)&size, sizeof(int));
                BlockCatogary::Catogary types[4096];
                for (int i = 0; i<256; i++) types[i*16 + offset] = BlockCatogary::Grass;
                file.write((char*)&types, sizeof(BlockCatogary::Catogary)*4096);
                file.close();
                std::lock_guard<std::mutex> lock(*mtx);
                *percent += one_part*total;
            }
        }
    }
    void MapCreator::createBedrockLayer(const std::string& src, const int& height) {
        unsigned int size = 1;
        for (int i = -40; i<40; i++) {
            for (int j = -40; j<40; j++) {
                glm::vec3 position = glm::vec3(i,j,height);
                std::ofstream file(getFileName(src, position), std::ios::out | std::ios::binary);
                position*=16;
                file.write((char*)&position, sizeof(glm::vec3));
                file.write((char*)&size, sizeof(int));

                BlockCatogary::Catogary type = BlockCatogary::Dirt;
                file.write((char*)&type, sizeof(BlockCatogary::Catogary));
                size = 256;
                file.write((char*)&size, sizeof(int));

                unsigned int buffer[128];
                for (int i = 0; i<128; i++) buffer[i] = 1<<31 | 1<<15;
                file.write((char*)&buffer[0], 128*sizeof(int));

                BlockCatogary::Catogary types[16];
                memset(&types[0], 0, 16*sizeof(BlockCatogary::Catogary));
                types[15] = BlockCatogary::Dirt;
                for (int i = 0; i<256; i++) file.write((char*)&types, 16*sizeof(BlockCatogary::Catogary));
                file.close();
            }
        }
    }
    void MapCreator::create(std::mutex* mtx, double* percent, const std::string& src) {
        // createBedrockLayer(src, -1);
        createPlaneSurface(mtx, percent, 1, src, -1);
    }
}
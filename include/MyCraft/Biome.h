#ifndef BIOME_H
#define BIOME_H
#include "Global.h"
#include "HeightMap.h"
namespace MyCraft {

    struct Biome {
        enum BiomeType: unsigned char {
            Null, SuperLow, Low, Mid, High, SuperHigh, Sea, Beach, MixRockyHill, RockyHill, Desert, Oasis, MixOasis, Lake, UnderGround
        };
        unsigned int height;
        BiomeType type;
    };
    std::string to_string(const Biome::BiomeType& type);
    class Biomes {
    public:
        Biomes(const unsigned int& width, const unsigned int& height);
        Biomes(Biomes&& biome);
        Biomes(const HeightMap& map);
        ~Biomes();
        Biomes(const Biomes&) = delete;
        Biomes& operator=(const Biomes&) const = delete; 
        const glm::ivec2& getSize() const;
        Biome& getBiome(const unsigned int& x, const unsigned int& y);
        const Biome& getBiome(const unsigned int& x, const unsigned int& y) const;
        void filter(std::vector<std::vector<glm::ivec2>>* list);
    protected:
    private:
        Biome** __biome;
        glm::ivec2 __size;
    };
    class BiomeManage {
    public:
        BiomeManage(const BiomeManage&) = delete;
        BiomeManage& operator=(const BiomeManage&) const = delete; 
        BiomeManage(const std::string& src);
        ~BiomeManage();
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        void setBiomeType(const glm::ivec3& position, const Biome::BiomeType& type);
        void setBiomeHeight(const glm::ivec3& position, const unsigned int& height);
        const Biome& getBiome(const glm::ivec3& position);
    protected:
    private:
        bool            __isLoad, __isChanged;
        glm::ivec3      __position;
        std::string     __source;
        Biome       __store[16][16][16];
        void __load(const glm::ivec3& file);
        void __save();
        Biome& get(const glm::ivec3& position);
    };
}
#endif
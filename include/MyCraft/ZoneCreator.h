#ifndef ZONE_CREATOR_H
#define ZONE_CREATOR_H
#include "Biome.h"
#include "Chunk.h"
#include "Global.h"
#include "HeightMap.h"
#include "Image.h"

namespace MyCraft {
    class Zone {
    public:
        virtual int getTectonicPerArea() const = 0;
        virtual void AnalysSurface(std::vector<glm::ivec2>&, HeightMap& map, Biomes& biome) const = 0;
        virtual void FillColumn(Chunk& chunk, HeightMap& map, Biome& biome, const glm::ivec3& ceiling) const = 0;
        virtual void FillChunk(const std::string& src, MyBase::Image& image, HeightMap& map, const glm::ivec3& origin, const Biome& type) const = 0;
    protected:
    private:
    };

    class Tropical: public Zone {
    public:
        Tropical() = default;
        Tropical(const Tropical&) = delete;
        Tropical& operator=(const Tropical&) const = delete; 

        int getTectonicPerArea() const override;
        void createOasisTree(DynamicChunk& chunk, const glm::ivec3& position) const;
        void FillColumn(Chunk& chunk, HeightMap& map, Biome& biome, const glm::ivec3& ceiling) const override;
        void FillChunk(const std::string& src, MyBase::Image& image, HeightMap& map, const glm::ivec3& origin, const Biome& type) const override;
        void AnalysSurface(std::vector<glm::ivec2>& spawner, HeightMap& map, Biomes& biome) const override;
    protected:
    private:
    };

    class Temperate: public Zone {
    public:
        Temperate() = default;
        Temperate(const Temperate&) = delete;
        Tropical& operator=(const Temperate&) const = delete; 

        int getTectonicPerArea() const override;
        void createOakTree(DynamicChunk& chunk, const glm::ivec3& position) const;
        void FillColumn(Chunk& chunk, HeightMap& map, Biome& biome, const glm::ivec3& ceiling) const override;
        void FillChunk(const std::string& src, MyBase::Image& image, HeightMap& map, const glm::ivec3& origin, const Biome& type) const override;
        void AnalysSurface(std::vector<glm::ivec2>& spawner, HeightMap& map, Biomes& biome) const override;
    protected:
    private:
    };
}
#endif
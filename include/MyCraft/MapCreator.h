#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H
#include "Chunk.h"
#include "Color.h"
#include "Global.h"
#include "Biome.h"
#include "Image.h"
#include "HeightMap.h"
#include "ZoneCreator.h"
namespace MyCraft {

    class MapCreator {
    public:
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        static void create(double* percent, const std::string& src);
    private:
        double* percent;
        std::string source;
        std::vector<glm::ivec3> spawner;
        MyBase::Image* image;

        MapCreator(double* percent, const std::string& src);


        void createBedrockLayer(const double& total, const int& heightBound);
        static void createSubBedrockLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        void createMagmaLayer(const double& total, const int& heightBound);
        static void createSubMagmaLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        void createTopSoilLayer(const double& total, const glm::ivec2& zBound);
        static void createSubTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound);

        static void createSubZone(const Zone* zone, double* percent, const double& total, Biomes* biome, MyBase::Image* image,const std::string& src, HeightMap* map, const glm::vec2& xBound, const glm::vec2& yBound, const int& heightOrigin);
        void createZone(const Zone& zone, const double& total, const glm::vec2& yBound, const int& z);

        // static void createPolarZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);
        // static void createTropicalZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);

    };
}
#endif
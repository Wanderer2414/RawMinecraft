#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H
#include "Color.h"
#include "Global.h"
namespace MyCraft {
    enum class Biome: unsigned char {
        Air, Sea, Beach, Low, MixRockyHill, Mid,RockyHill, High, Desert, Oasis, MixOasis
    };
    class MapCreator {
    public:
        MapCreator() = delete;
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        static void create(double* percent, const std::string& src);
    private:
        static void createBedrockLayer(double* percent, const double& total, const std::string& src, const int& heightBound);
        static void createSubBedrockLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        static void createMagmaLayer(double* percent, const double& total, const std::string& src, const int& heightBound);
        static void createSubMagmaLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        static void createTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& zBound);
        static void createSubTopSoilLayer(double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound);

        static void toBiome(Biome**&, unsigned int** board, const glm::ivec2& size);
        static void createSubTemperateZone(double* percent, const double& total, Biome** biome, MyBase::Color* color,const std::string& src, const glm::vec2& bound, unsigned int** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin);
        static void createTemperateZone(double* percent, const double& total, std::vector<glm::vec2>& centers, MyBase::Color* color, const std::string& src, const glm::vec2& yBound, const float& z);

        static void createPolarZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);
        static void createTropicalZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);

    };
}
#endif
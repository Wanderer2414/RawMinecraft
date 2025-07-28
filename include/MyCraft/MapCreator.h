#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H
#include "Color.h"
#include "Global.h"
namespace MyCraft {
    class MapCreator {
    public:
        MapCreator() = delete;
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        static void create(std::mutex* mtx, double* percent, const std::string& src);
    private:
        static void createBedrockLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound);
        static void createSubBedrockLayer(std::mutex* mtx,double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        static void createMagmaLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound);
        static void createSubMagmaLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);

        static void createTopSoilLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& zBound);
        static void createSubTopSoilLayer(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& zBound);

        static void RenderToMap(MyBase::Color* texture, unsigned char** map, const glm::ivec2& xBound, const glm::ivec2& yBound, const glm::ivec2& origin);
        static void createSubTemperateZone(std::mutex* mtx, double* percent, const double& total, const std::string& src, const glm::vec2& bound, unsigned char** board, const glm::vec2& xBound, const glm::vec2& yBound, const glm::ivec3& origin);
        static void createTemperateZone(std::mutex* mtx, double* percent, const double& total, std::vector<glm::vec2>& centers, MyBase::Color* color, const std::string& src, const glm::vec2& yBound, const float& z);

        static void createPolarZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);
        static void createTropicalZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);

    };
}
#endif
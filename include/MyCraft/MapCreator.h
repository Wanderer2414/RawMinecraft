#ifndef MAP_CREATOR_H
#define MAP_CREATOR_H
#include "Global.h"
namespace MyCraft {
    class MapCreator {
    public:
        MapCreator() = delete;
        static std::string getFileName(const std::string& src, const glm::ivec3& position);
        static void create(std::mutex* mtx, double* percent, const std::string& src);
    private:
        static void createPlaneSurface(std::mutex* mtx, double* percent, const double& total, const std::string& src, const int& heightBound);
        static void createSubPlaneSurface(std::mutex* mtx,double* percent, const double& total, const std::string& src, const glm::ivec2& xBound, const glm::ivec2& yBound, const int& height);
        static void createBedrockLayer(const std::string& src, const int& heightBound);
        static void createMagmaLayer(const std::string& src, const glm::vec2& heightBound);
        static void createTopsoilLayer(const std::string& src, const glm::vec2& heightBound);
        static void createPolarZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);
        static void createTemperateZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);
        static void createTropicalZone(const std::string& src, const glm::vec2& yBound, const glm::vec2& heightBound);

    };
}
#endif
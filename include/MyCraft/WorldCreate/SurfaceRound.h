#ifndef Round_H
#define Round_H
#include "Biome.h"
#include "Global.h"
#include "HeightMap.h"
namespace MyCraft {
    class Biome;
    class PorlarVector2 {
    public:
        PorlarVector2();
        PorlarVector2(const float& l, const float& a);
        PorlarVector2(const glm::vec2& vec);
        float length;
        float angle;
        operator glm::vec2() const;
    };

    class Round {
    private:
        std::vector<PorlarVector2> vertices;
        glm::vec2 origin;
        glm::vec2 size;
    public:
        Round(const size_t& n, const glm::vec2& s);
        Round();
        Round(const glm::vec2& size);
        Round(const Round& Round);
        bool empty() const;
        float operator[](float angle) const;
        Round& operator=(const Round& Round);
        Round operator+(const float& max_width) const;
        Round operator-(const float& max_width) const;
        bool intersect(const Round& Round) const;
        glm::vec2 getCenter() const;
        void setPosition(const glm::vec2& p);
        void setRoundness(const size_t& size);
        void applyRound(HeightMap& map) const;
        void applyLake(Biomes* biome, HeightMap& map) const;
    private:
        void __intersect(std::vector<glm::vec2>& vecs, const Round& Round) const;
        void __merge(std::vector<glm::vec2>& vecs, const Round& Round) const;
    };
    class Area {
        glm::vec2 size, origin;
        std::vector<Round*> Rounds;
        std::vector<Round*> specials;
    public:
        Area();
        Area(const int& n, const glm::vec2& org, const glm::vec2& s);
        Area(const Area&) = delete;
        ~Area();
        std::vector<glm::vec2> getCenter() const;
        Area& operator=(const Area&) const = delete;
        void applyRounds(HeightMap& map) const;
    };
}
#endif
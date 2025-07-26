#ifndef TECTONIC_H
#define TECTONIC_H
#include "Global.h"
namespace MyCraft {

    class PorlarVector2 {
    public:
        PorlarVector2();
        PorlarVector2(const float& l, const float& a);
        PorlarVector2(const glm::vec2& vec);
        float length;
        float angle;
        operator glm::vec2() const;
    };

    class Tectonic {
    private:
        std::vector<PorlarVector2> vertices;
        glm::vec2 origin;
        glm::vec2 size;
    public:
        Tectonic(const size_t& n, const glm::vec2& s);
        Tectonic();
        Tectonic(const glm::vec2& size);
        Tectonic(const Tectonic& tectonic);
        bool empty() const;
        float operator[](float angle) const;
        Tectonic& operator=(const Tectonic& tectonic);
        Tectonic operator+(const float& max_width) const;
        Tectonic operator-(const float& max_width) const;
        bool intersect(const Tectonic& Tectonic) const;
        void setPosition(const glm::vec2& p);
        void setRoundness(const size_t& size);
        void draw(const glm::vec2& size, unsigned char** board) const;
    private:
        void __intersect(std::vector<glm::vec2>& vecs, const Tectonic& Tectonic) const;
        void __merge(std::vector<glm::vec2>& vecs, const Tectonic& Tectonic) const;
    };
    class Area {
        glm::vec2 size, origin;
        std::vector<Tectonic*> Tectonics;
        std::vector<Tectonic*> specials;
    public:
        Area();
        Area(const int& n, const glm::vec2& org, const glm::vec2& s);
        Area(const Area&) = delete;
        ~Area();
        Area& operator=(const Area&) const = delete;
        void draw(const glm::vec2& size, unsigned char** board) const;
    };
}
#endif
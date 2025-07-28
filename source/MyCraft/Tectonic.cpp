#include "Tectonic.h"
#include "glm/fwd.hpp"
#include <cstdlib>

namespace MyCraft {

    PorlarVector2::PorlarVector2(): length(0),angle(0) {};
    PorlarVector2::PorlarVector2(const float& l, const float& a):length(l), angle(a) {};
    PorlarVector2::PorlarVector2(const glm::vec2& vec) {
        angle = atan(vec.y/vec.x);
        if (vec.x<0) angle += M_PI;
        if (angle<0) angle+=M_PI*2;
        if (angle>M_PI*2) angle-=M_PI*2;
        length =sqrt(vec.x*vec.x + vec.y*vec.y);
    }
    PorlarVector2::operator glm::vec2() const {
        return {length*cos(angle), length*sin(angle)};
    }

    Tectonic::Tectonic(const size_t& n, const glm::vec2& s): origin({0,0}), size(s) {
        float remain = 100;
        for (int i = 0; i<n; i++) {
            float pecent = rand()%50+10;
            pecent = pecent*i/n;
            float angle = 100-remain + pecent*remain/100;
            remain = 100-angle;
            angle *= M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*(size.x*abs(cos(angle)) + size.y*abs(sin(angle)))/2/sqrt(2);
            vertices.push_back({length, angle});
        }
        if (n && remain>25) {
            float percent = rand()%50+25;
            percent = (remain*percent/100 + 100 -remain)*M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*(size.x*abs(cos(percent)) + size.y*abs(sin(percent)))/2;
            vertices.push_back({length, percent});
        }
    }
    Tectonic::Tectonic(): Tectonic(0, {0,0}) {};
    Tectonic::Tectonic(const glm::vec2& size): Tectonic(rand()%10+5, size) {}
    Tectonic::Tectonic(const Tectonic& tectonic) {
        vertices = tectonic.vertices;
        origin = tectonic.origin;
        size = tectonic.size;
    }
    bool Tectonic::empty() const {
        return vertices.empty();
    }
    float Tectonic::operator[](float angle) const {
        while (angle<0) angle += M_PI*2;
        while (angle>M_PI*2) angle -= M_PI*2;
        int i = 0;
        while (i<vertices.size() && angle>vertices[i].angle) i++;
        PorlarVector2 above, below;
        if (i>0) {
            below = vertices[i-1];
        }
        else {
            below = vertices.back();
            below.angle -= M_PI*2;
        }
        if (i<vertices.size()) {
            above = vertices[i];
        }
        else {
            above = vertices[0];
            above.angle += M_PI*2;
        }
        float dem = sin(angle-below.angle)/sin(above.angle-below.angle)/above.length +
            sin(above.angle - angle)/sin(above.angle-below.angle)/below.length;

        return 1.f/dem;
    }
    Tectonic& Tectonic::operator=(const Tectonic& tectonic) {
        if (&tectonic != this) {
            vertices = tectonic.vertices;
            origin = tectonic.origin;
            size = tectonic.size;
        }
        return *this;
    }
    Tectonic Tectonic::operator+(const float& max_width) const {
        if (max_width<0) return operator-(max_width);
        Tectonic Tectonic;
        Tectonic.vertices.resize(vertices.size());
        Tectonic.origin = origin;
        Tectonic.size = {size.x+max_width*2, size.y+max_width*2};
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<vertices.size(); i++) {
            float angle = M_PI*2*(i+1)/vertices.size();
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            Tectonic.vertices[i] = {operator[](angle) + width, angle};
        }
        return  Tectonic;
    }
    Tectonic Tectonic::operator-(const float& max_width) const {
        if (max_width<0) return operator+(max_width);
        Tectonic Tectonic;
        Tectonic.vertices.resize(vertices.size());
        Tectonic.origin = origin;
        Tectonic.size = {size.x-max_width*2, size.y - max_width*2};
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<vertices.size(); i++) {
            float angle = M_PI*2*(i+1)/vertices.size();
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            Tectonic.vertices[i] = {operator[](angle) - width, angle};
        }
        return Tectonic;
    }
    bool Tectonic::intersect(const Tectonic& Tectonic) const {
        for (glm::vec2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            glm::vec2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)>distance) 
                return true;
        }
        return false;
    }
    glm::vec2 Tectonic::getCenter() const {
        return origin;
    }
    void Tectonic::setPosition(const glm::vec2& p) {
        origin = p;
    }
    void Tectonic::setRoundness(const size_t& size) {
        float old_percent = 1.f/vertices.size();
        float new_percent = 1.f/size;
        std::vector<PorlarVector2> buffer(size);
        for (int i = 0; i<size; i++) {
            float percent = i*new_percent/old_percent;
            int index = floor(percent);
            float a = percent-index, b = 1-a;
            buffer[i].angle = i*new_percent*M_PI*2;
            float distance = operator[](buffer[i].angle);
            if (index<vertices.size()-1) {
                buffer[i].length = vertices[index+1].length*b + vertices[index].length*a;
            }
            else {
                buffer[i].length = vertices[0].length*b + vertices[index].length*a;
            }
            buffer[i].length = (buffer[i].length - distance)*0.5 + distance;
        }
        vertices = buffer;
    }
    void Tectonic::draw(const glm::vec2& size, unsigned char** board) const {
        {
            Tectonic outside = *this;
            outside.setRoundness(50);
            bool reachBound = false;
            for (int i = 0; i<32 && !reachBound; i++) {
                outside = outside + 10;
                int xSz = ceil(outside.size.x);

                glm::vec2 *bounds = new glm::vec2[xSz];
                for (int i = 0; i<xSz; i++) bounds[i] = {outside.size.y/2, -outside.size.y/2};
                for (int i = 0; i<outside.vertices.size(); i++) {
                    glm::vec2 a, b = outside.vertices[i], delta;
                    if (i) a = outside.vertices[i-1];
                    else a = outside.vertices.back();
                    delta = b-a;
                    delta.y /= delta.x;
                    delta.x = 1;
                    if (a.x > b.x) std::swap(a , b);
                    for (glm::vec2 vec(floor(a.x), a.y); vec.x < floor(b.x); vec = vec + delta) {
                        int index = vec.x + outside.size.x/2;
                        if (index>=0 && index<xSz) {
                            bounds[index].x = std::min(vec.y, bounds[index].x);
                            bounds[index].y = std::max(vec.y, bounds[index].y);
                        }
                    }
                }
                for (int i = 0; i<outside.size.x; i++) {
                    if (i<xSz && (bounds[i].x < bounds[i].y)) {
                        for (int j = bounds[i].x + outside.origin.y; j < bounds[i].y + outside.origin.y; j++) {
                            int x = int(i-outside.size.x/2+ outside.origin.x);
                            if (x>=0 && x<size.x && j>=0 && j<=size.y) board[x][j]++;
                            else reachBound = true;
                        }
                    }
                }
                delete[] bounds;
                bounds = 0;
            }
        }
        {
            Tectonic outside = *this;
            outside.setRoundness(50);
            for (int i = 0; i<20; i++) {
                outside = outside - 30;
                int xSz = ceil(outside.size.x);
                if (xSz<=50) break;
                glm::vec2 *bounds = new glm::vec2[xSz];
                for (int i = 0; i<xSz; i++) bounds[i] = {outside.size.y/2, -outside.size.y/2};
                for (int i = 0; i<outside.vertices.size(); i++) {
                    glm::vec2 a, b = outside.vertices[i], delta;
                    if (i) a = outside.vertices[i-1];
                    else a = outside.vertices.back();
                    delta = b-a;
                    delta.y /= delta.x;
                    delta.x = 1;
                    if (a.x > b.x) std::swap(a , b);
                    for (glm::vec2 vec(floor(a.x), a.y); vec.x < floor(b.x); vec = vec + delta) {
                        int index = vec.x + outside.size.x/2;
                        if (index>=0 && index<xSz) {
                            bounds[index].x = std::min(vec.y, bounds[index].x);
                            bounds[index].y = std::max(vec.y, bounds[index].y);
                        }
                    }
                }
                for (int i = 0; i<outside.size.x; i++) {
                    if (i<xSz && (bounds[i].x < bounds[i].y))
                        for (int j = bounds[i].x + outside.origin.y; j < bounds[i].y + outside.origin.y; j++) {
                            int x = int(i-outside.size.x/2+ outside.origin.x);
                            if (x>=0 && x<size.x && j>=0 && j<=size.y) board[x][j]++;
                        }
                }
                delete[] bounds;
                bounds = 0;
            }
        }
    }
    void Tectonic::__intersect(std::vector<glm::vec2>& vecs, const Tectonic& Tectonic) const {
        for (glm::vec2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            glm::vec2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)>distance)
                vecs.push_back(position);
        }
    }
    void Tectonic::__merge(std::vector<glm::vec2>& vecs, const Tectonic& Tectonic) const {
        for (glm::vec2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            glm::vec2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)<=distance) 
                vecs.push_back(position);
        }
    }
    Area::Area() {
        srand(clock());
    }
    Area::Area(const int& n, const glm::vec2& org, const glm::vec2& s): origin(org), size(s) {
        Tectonics.resize(n);
        float remain = 1;
        for (int i = 0; i<n; i++) {
            float percent = 1.0f*(rand()%70)/100 + 0.25;
            glm::vec2 subsize =  {10.0/n*percent*s.y, percent*s.y*n/15.0};
            Tectonics[i] = new Tectonic(20, subsize);
            glm::vec2 position;
            percent = 1.0f*(rand()%30)/100;
            percent = percent*remain + 1 - remain;
            remain = (1 - percent)*0.7;
            position.x = (size.x-subsize.x)*percent+subsize.x/2 + origin.x;
            percent = 1.0f*(rand()%100)/100;
            position.y = (size.y-subsize.y)*percent + subsize.y/2 + origin.y;
            Tectonics[i]->setPosition(position);
        }
    }
    Area::~Area() {
        for (auto& Tectonic: Tectonics) delete Tectonic;
        for (auto& Tectonic: specials) delete Tectonic;
        Tectonics.clear();
        specials.clear();
    }
    std::vector<glm::vec2> Area::getCenter() const {
        std::vector<glm::vec2> ans(Tectonics.size());
        for (int i = 0; i<Tectonics.size(); i++) ans[i] = Tectonics[i]->getCenter();
        return ans;
    }
    void Area::draw(const glm::vec2& size, unsigned char** board) const {
        for (auto& Tectonic: Tectonics) Tectonic->draw(size, board);
    }
};
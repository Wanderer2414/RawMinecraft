#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <vector>

Vector2 operator+(const Vector2& a, const Vector2& b) {
    return {a.x+b.x, a.y+b.y};
}
Vector2 operator-(const Vector2& a, const Vector2& b) {
    return {a.x-b.x, a.y-b.y};
}
float length(const Vector2& vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}
class PorlarVector2 {
public:
    PorlarVector2(): length(0),angle(0) {};
    PorlarVector2(const float& l, const float& a):length(l), angle(a) {};
    PorlarVector2(const Vector2& vec) {
        angle = atan(vec.y/vec.x);
        if (vec.x<0) angle += M_PI;
        if (angle<0) angle+=M_PI*2;
        if (angle>M_PI*2) angle-=M_PI*2;
        length =sqrt(vec.x*vec.x + vec.y*vec.y);
    }
    float length;
    float angle;
    operator Vector2() const {
        return {length*cos(angle), length*sin(angle)};
    }
};

class HeightRound {
private:
    std::vector<PorlarVector2> vertices;
    Vector2 origin;
    float __size;
public:
    HeightRound(const size_t& n, const float& size): origin({0,0}), __size(size) {
        float remain = 100;
        for (int i = 0; i<n; i++) {
            float pecent = rand()%50+10;
            pecent = pecent*i/n;
            float angle = 100-remain + pecent*remain/100;
            remain = 100-angle;
            angle *= M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*size;
            vertices.push_back({length, angle});
        }
        if (n && remain>25) {
            float percent = rand()%50+25;
            percent = (remain*percent/100 + 100 -remain)*M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*size;
            vertices.push_back({length, percent});
        }
    }
    HeightRound(): HeightRound(0, 0) {};
    HeightRound(const float& size): HeightRound(rand()%10+5, size) {}
    float operator[](float angle) const {
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
    HeightRound operator+(const float& max_width) const {
        if (max_width<0) return operator-(max_width);
        HeightRound heightRound;
        heightRound.vertices.resize(vertices.size());
        heightRound.origin = origin;
        heightRound.__size = __size+max_width;
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<vertices.size(); i++) {
            float angle = M_PI*2*(i+1)/vertices.size();
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            heightRound.vertices[i] = {operator[](angle) + width, angle};
        }
        return  heightRound;
    }
    HeightRound operator-(const float& max_width) const {
        if (max_width<0) return operator+(max_width);
        HeightRound heightRound;
        heightRound.vertices.resize(vertices.size());
        heightRound.origin = origin;
        heightRound.__size = __size - max_width;
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<vertices.size(); i++) {
            float angle = M_PI*2*(i+1)/vertices.size();
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            heightRound.vertices[i] = {operator[](angle) - width, angle};
        }
        return heightRound;
    }
    HeightRound operator+(const HeightRound& round) const {

        std::vector<Vector2> ans;
        __merge(ans, round);
        round.__merge(ans, *this);
        Vector2 centroid = {0,0}, TopLeft={0,0}, BottomRight={0,0};
        for (auto& vec:ans) {
            centroid=centroid+vec;
            TopLeft.x = std::min(vec.x, TopLeft.x);
            TopLeft.y = std::min(vec.y, TopLeft.y);
            BottomRight.x = std::max(vec.x, BottomRight.x);
            BottomRight.y = std::max(vec.y, BottomRight.y);
        }
        centroid = {centroid.x/ans.size(), centroid.y/ans.size()};
        HeightRound roundans;
        roundans.setPosition(centroid);
        roundans.__size = std::max(BottomRight.x-TopLeft.x, BottomRight.y-TopLeft.y)/2;
        for (auto& vec:ans) {
            PorlarVector2 pVec(vec-centroid);
            if (roundans.vertices.empty()) roundans.vertices.push_back(pVec);
            else {
                int i = 0; 
                while (i<roundans.vertices.size() && roundans.vertices[i].angle<pVec.angle) i++;
                if (i==roundans.vertices.size()) roundans.vertices.push_back(pVec);
                else roundans.vertices.insert(roundans.vertices.begin()+i, pVec);
            }
        }
        return roundans;
    }
    HeightRound operator-(const HeightRound& round) const {
        std::vector<Vector2> vecs;
        __intersect(vecs, round);
        round.__intersect(vecs, *this);
        Vector2 centroid = {0,0}, TopLeft={0,0}, BottomRight={0,0};
        for (auto& vec:vecs) {
            centroid=centroid+vec;
            TopLeft.x = std::min(vec.x, TopLeft.x);
            TopLeft.y = std::min(vec.y, TopLeft.y);
            BottomRight.x = std::max(vec.x, BottomRight.x);
            BottomRight.y = std::max(vec.y, BottomRight.y);
        }
        centroid = {centroid.x/vecs.size(), centroid.y/vecs.size()};
        HeightRound ans;
        ans.origin = centroid;
        ans.__size = std::max(BottomRight.x-TopLeft.x, BottomRight.y-TopLeft.y)/2;
        for (auto& vec:vecs) {
            PorlarVector2 pVec(vec-centroid);
            if (ans.vertices.empty()) ans.vertices.push_back(pVec);
            else {
                int i = 0; 
                while (i<ans.vertices.size() && ans.vertices[i].angle<pVec.angle) i++;
                if (i==ans.vertices.size()) ans.vertices.emplace_back(pVec);
                else ans.vertices.emplace(ans.vertices.begin()+i, pVec);
            }
        }
        return ans;
    }
    void setPosition(const Vector2& p) {
        origin = p;
    }
    void setRoundness(const size_t& size) {
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
    void draw() const {
        for (int i = 0; i<vertices.size()-1; i++) 
            DrawLineEx(origin+vertices[i], vertices[i+1]+origin, 2, WHITE);
        DrawLineEx(vertices[0]+origin, vertices.back()+origin, 2, WHITE); 
        DrawCircleV(origin, 5, RED);
    }
private:
    void __intersect(std::vector<Vector2>& vecs, const HeightRound& round) const {
        for (Vector2 position: round.vertices) {
            position =position + round.origin;
            Vector2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)>distance) 
                vecs.push_back(position);
        }
    }
    void __merge(std::vector<Vector2>& vecs, const HeightRound& round) const {
        for (Vector2 position: round.vertices) {
            position =position + round.origin;
            Vector2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)<=distance) 
                vecs.push_back(position);
        }
    }
};

int main() {
    InitWindow(1000, 1000, "World Render");
    srand(clock());
    HeightRound roundA(5, 500), roundB(5, 500);
    roundA.setPosition({500,500});
    roundB.setPosition({750, 500});
    HeightRound roundC = roundA-roundB;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        roundB.draw();
        roundA.draw();
        roundC.draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}

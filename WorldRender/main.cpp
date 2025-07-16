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
public:
    void randomCreate(const size_t& count, const float& size);
    void draw();

};
class Tectonic {
private:
    float __size;
    Vector2 origin;
    std::vector<PorlarVector2> vertices;
    PorlarVector2 outSideBound[50];
    PorlarVector2 insideBound[20];
public:

    Tectonic(const Vector2& position, const float& size): origin(position), __size(size) {
        srand(clock());
    }
    ~Tectonic() {}
    friend void __intersect(std::vector<Vector2>& vecs, const Tectonic&, const Tectonic&);
    friend void __merge(std::vector<Vector2>& vecs, const Tectonic&, const Tectonic&);

    void createVertices() {
        int n = rand()%10+5;
        float remain = 100;
        for (int i = 0; i<n; i++) {
            float pecent = rand()%50+10;
            pecent = pecent*i/n;
            float angle = 100-remain + pecent*remain/100;
            remain = 100-angle;
            angle *= M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*__size;
            vertices.push_back({length, angle});
        }
        if (remain>25) {
            float percent = rand()%50+25;
            percent = (remain*percent/100 + 100 -remain)*M_PI*2/100;
            float length = rand()%25+75;
            length = length/100*__size;
            vertices.push_back({length, percent});
        }
    }
    void createOutside(const float& max_width) {
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<50; i++) {
            float angle = M_PI*2*i/50;
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            outSideBound[i] = {minDistance(angle) + width, angle};
        }
    }
    void createInside(const float& max_width) {
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<20; i++) {
            float angle = M_PI*2*i/20;
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            insideBound[i] = {minDistance(angle) - width, angle};
        }
    }
    float minDistance(float angle) const {
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
    void draw() const {
        // for (int i = 0; i<vertices.size()-1; i++) 
        //     DrawLineEx(origin+vertices[i], vertices[i+1]+origin, 2, WHITE);
        // DrawLineEx(vertices[0]+origin, vertices.back()+origin, 2, WHITE); 
        for (int i = 0; i<49; i++) {
            DrawLineEx(outSideBound[i] + origin,outSideBound[i+1]+origin, 2, RED);
        }
        DrawLineEx(outSideBound[0] + origin,outSideBound[49]+origin, 2, RED);

        for (int i = 0; i<19; i++) {
            DrawLineEx(insideBound[i] + origin,insideBound[i+1]+origin, 2, RED);
        }
        DrawLineEx(insideBound[0] + origin,insideBound[19]+origin, 2, RED);
    }
    friend Tectonic operator+(const Tectonic& a, const Tectonic& b);
    friend Tectonic operator-(const Tectonic& a, const Tectonic& b);
};

void __intersect(std::vector<Vector2>& vecs, const Tectonic& a, const Tectonic& b) {
    for (Vector2 position: b.vertices) {
        position =position + b.origin;
        Vector2 delta = position - a.origin;
        float angle = atan(delta.y/delta.x);
        if (delta.x<0) angle+=M_PI;
        float distance = length(delta);
        if (a.minDistance(angle)>distance) 
            vecs.push_back(position);
    }
}
void __merge(std::vector<Vector2>& vecs, const Tectonic& a, const Tectonic& b) {
    for (Vector2 position: b.vertices) {
        position =position + b.origin;
        Vector2 delta = position - a.origin;
        float angle = atan(delta.y/delta.x);
        if (delta.x<0) angle+=M_PI;
        float distance = length(delta);
        if (a.minDistance(angle)<=distance) 
            vecs.push_back(position);
    }
}
Tectonic operator+(const Tectonic& a, const Tectonic& b) {
    std::vector<Vector2> ans;
    __merge(ans, a, b);
    __merge(ans, b, a);
    Vector2 centroid = {0,0}, TopLeft={0,0}, BottomRight={0,0};
    for (auto& vec:ans) {
        centroid=centroid+vec;
        TopLeft.x = std::min(vec.x, TopLeft.x);
        TopLeft.y = std::min(vec.y, TopLeft.y);
        BottomRight.x = std::max(vec.x, BottomRight.x);
        BottomRight.y = std::max(vec.y, BottomRight.y);
    }
    centroid = {centroid.x/ans.size(), centroid.y/ans.size()};
    Tectonic tectonic(centroid, std::max(BottomRight.x-TopLeft.x, BottomRight.y-TopLeft.y)/2);
    for (auto& vec:ans) {
        PorlarVector2 pVec(vec-centroid);
        if (tectonic.vertices.empty()) tectonic.vertices.push_back(pVec);
        else {
            int i = 0; 
            while (i<tectonic.vertices.size() && tectonic.vertices[i].angle<pVec.angle) i++;
            if (i==tectonic.vertices.size()) tectonic.vertices.push_back(pVec);
            else tectonic.vertices.insert(tectonic.vertices.begin()+i, pVec);
        }
    }
    return tectonic;
}

Tectonic operator-(const Tectonic& a, const Tectonic& b) {
    std::vector<Vector2> ans;
    __intersect(ans, a, b);
    __intersect(ans, b, a);
    Vector2 centroid = {0,0}, TopLeft={0,0}, BottomRight={0,0};
    for (auto& vec:ans) {
        centroid=centroid+vec;
        TopLeft.x = std::min(vec.x, TopLeft.x);
        TopLeft.y = std::min(vec.y, TopLeft.y);
        BottomRight.x = std::max(vec.x, BottomRight.x);
        BottomRight.y = std::max(vec.y, BottomRight.y);
    }
    centroid = {centroid.x/ans.size(), centroid.y/ans.size()};
    Tectonic tectonic(centroid, std::max(BottomRight.x-TopLeft.x, BottomRight.y-TopLeft.y)/2);
    for (auto& vec:ans) {
        PorlarVector2 pVec(vec-centroid);
        if (tectonic.vertices.empty()) tectonic.vertices.push_back(pVec);
        else {
            int i = 0; 
            while (i<tectonic.vertices.size() && tectonic.vertices[i].angle<pVec.angle) i++;
            if (i==tectonic.vertices.size()) tectonic.vertices.push_back(pVec);
            else tectonic.vertices.insert(tectonic.vertices.begin()+i, pVec);
        }
    }
    return tectonic;
}
int main() {
    Tectonic tectonicA({500,500}, 250), tectonicB({750, 500}, 250),
            tectonicC({900, 200}, 250);
    tectonicA.createVertices(); tectonicB.createVertices(); tectonicC.createVertices();
    InitWindow(1500, 1000, "Raylib - Generated Texture");

    // Generate a 256x256 image with a checkerboard pattern
    Image image = GenImageColor(256, 256, WHITE);

    // Convert the image into a texture
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    auto tectonic = tectonicA+ tectonicB +tectonicC;
    auto tectonicAB = tectonicA-tectonicB, tectonicBC = tectonicB-tectonicC;
    tectonic.createInside(50);
    // mTectonic.createInside(50);
    tectonic.createOutside(200); 
    // mTectonic.createOutside(50);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // tectonicA.draw();
        // tectonicB.draw();
        // tectonicC.draw();
        // tectonicA + tectonicB;
        // mTectonic.draw();
        tectonic.draw();
        tectonicAB.draw();
        tectonicBC.draw();
        // DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}

#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <utility>
#include <vector>
Vector2 operator+(const Vector2& a, const Vector2& b) {
    return {a.x+b.x, a.y+b.y};
}
Vector2 operator-(const Vector2& a, const Vector2& b) {
    return {a.x-b.x, a.y-b.y};
}
class PorlarVector2 {
public:
    float length;
    float angle;
    operator Vector2() const {
        return {length*cos(angle), length*sin(angle)};
    }
};
float length(const Vector2& vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y);
}
class Tectonic {
private:
    float __size;
    std::vector<PorlarVector2> vertices;
    PorlarVector2 outSizeBound[100];
    void __createVertices() {
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
        vertices.push_back(vertices[0]);
        vertices.back().angle += M_PI*2;
    }
    void __createOutsideBound(const float& max_width) {
        float width = 1.0f*(rand()%50+25)/100*max_width;
        for (int i = 0; i<100; i++) {
            float angle = M_PI*2*i/100;
            if (width>0.2*max_width)
                width = width*(rand()%40-20+100)/100;
            else width = width*(rand()%20+100)/100;
            if (width>max_width) width = max_width;
            outSizeBound[i] = {minDistance(angle) + width, angle};
        }
    }
    float minDistance(float angle) const {
        while (angle<0) angle += M_PI*2;
        while (angle>M_PI*2) angle -= M_PI*2;
        int i = 0;
        while (angle>vertices[i].angle) i++;
        float below, above = vertices[i].angle, below_length, above_length = vertices[i].length;

        if (i>0) {
            below = vertices[i-1].angle;
            below_length = vertices[i-1].length;
        }
        else {
            below = vertices.back().angle-M_PI*2;
            below_length = vertices.back().length;
        }

        float distance = length(vertices[i]-vertices[i-1]);
        float dem = sin(angle-below)/sin(above-below)/above_length +
            sin(above - angle)/sin(above-below)/below_length;
        return 1.f/dem;
    }

public:
    Tectonic(const float& size): __size(size) {
        srand(clock());
        __createVertices();
        __createOutsideBound(200);
    }
    ~Tectonic() {}
    void draw(const Vector2& origin) const {
        for (int i = 0; i<vertices.size()-1; i++) 
            DrawLineEx(origin+vertices[i], vertices[i+1]+origin, 2, WHITE);
        DrawLineEx(vertices[0]+origin, vertices.back()+origin, 2, WHITE); 
        for (int i = 0; i<99; i++) {
            DrawLineEx(outSizeBound[i] + origin,outSizeBound[i+1]+origin, 2, RED);
        }
        DrawLineEx(outSizeBound[0] + origin,outSizeBound[99]+origin, 2, RED);
    }
};
int main() {
    Tectonic tectonic(250);
    InitWindow(1000, 1000, "Raylib - Generated Texture");

    // Generate a 256x256 image with a checkerboard pattern
    Image image = GenImageColor(256, 256, WHITE);

    // Convert the image into a texture
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        tectonic.draw({500, 500});
        // DrawTexture(texture, 0, 0, WHITE);

        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();

    return 0;
}

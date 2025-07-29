#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
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
void rasterize(std::vector<std::vector<bool>>& matrix, const Vector2& a, const Vector2& b, const Vector2& origin) {
    if (a.y>b.y) rasterize(matrix, b,a, origin);
    else {
        Vector2 delta = b-a;
        delta.x /= delta.y;
        delta.y = 1;
        for (int y = ceil(a.y); y<b.y; y++) {
            int x = a.x + (y-a.y)/delta.y*delta.x;
            matrix[floor(x+origin.x)][floor(y+origin.y)] = 1;
        }
    }
}
class Tectonic {
private:
    std::vector<PorlarVector2> vertices;
    std::vector<std::vector<bool>> matrix;
    Vector2 origin, position;
    Vector2 size;
    Tectonic(): Tectonic(0, {0,}) {};
public:
    void raster() {
        Vector2 xBound = {size.x, 0}, yBound = {size.y, 0};
        for (int i = 0; i<vertices.size(); i++) {
            xBound.x = std::min(xBound.x, Vector2(vertices[i]).x);
            xBound.y = std::max(xBound.y, Vector2(vertices[i]).x);
            yBound.x = std::min(yBound.x, Vector2(vertices[i]).y);
            yBound.y = std::max(yBound.y, Vector2(vertices[i]).y);
        }
        size = {ceil(xBound.y-xBound.x), ceil(yBound.y-yBound.x)};
        origin = {-xBound.x, -yBound.x};
        matrix.resize(size.x);
        for (int i = 0; i<size.x; i++) matrix[i].resize(size.y, 0);
        if (vertices.size())  {
            for (int i = 1; i<vertices.size(); i++) {
                rasterize(matrix, vertices[i-1], vertices[i], origin);
            }
            rasterize(matrix, vertices.back(), vertices[0], origin);
            for (int y = 0; y<size.y; y++) {
                bool isDraw = false;
                int minX = 0, maxX = floor(size.x)-1;
                while (minX<matrix.size() && !matrix[minX][y]) minX++;
                while (maxX>0 && !matrix[maxX][y]) maxX--;
                for (int x = minX; x<maxX; x++) {
                    if (matrix[x][y]) isDraw = !isDraw;
                    if (isDraw) matrix[x][y] = 1;
                }
            }
        }
    }
    Tectonic(const size_t& n, const Vector2& s): origin({0,0}), size(s) {
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
        raster();
    }
    Tectonic(const Tectonic& tectonic) {
        vertices = tectonic.vertices;
        origin = tectonic.origin;
        size = tectonic.size;
    }
    bool empty() const {
        return vertices.empty();
    }
    void setPosition(const Vector2& p) {
        position = p;
    }
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
    Tectonic& operator=(const Tectonic& tectonic) {
        if (&tectonic != this) {
            vertices = tectonic.vertices;
            origin = tectonic.origin;
            size = tectonic.size;
        }
        return *this;
    }
    Tectonic operator+(const float& max_width) const {
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
        Tectonic.raster();
        return  Tectonic;
    }
    Tectonic operator-(const float& max_width) const {
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
        Tectonic.raster();
        return Tectonic;
    }
    bool intersect(const Tectonic& Tectonic) const {
        for (Vector2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            Vector2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)>distance) 
                return true;
        }
        return false;
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
            DrawLineEx(position+vertices[i], vertices[i+1]+position, 2, WHITE);
        DrawLineEx(vertices[0]+position, vertices.back()+position, 2, WHITE); 
        DrawCircleV(position, 5, RED);
    }
    void draw(const Vector2& bound, unsigned char** board) const {
        for (int x = 0; x<matrix.size(); x++) {
            for (int y = 0; y<matrix[0].size(); y++) {
                int mX = x + position.x - origin.x, mY = y + position.y - origin.y;
                if (mX>=0 && mX<bound.x && mY>=0 && mY<bound.y)
                    if (matrix[x][y]) board[mX][mY]++;
            }
        }
    }
private:
    void __intersect(std::vector<Vector2>& vecs, const Tectonic& Tectonic) const {
        for (Vector2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            Vector2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)>distance)
                vecs.push_back(position);
        }
    }
    void __merge(std::vector<Vector2>& vecs, const Tectonic& Tectonic) const {
        for (Vector2 position: Tectonic.vertices) {
            position =position + Tectonic.origin;
            Vector2 delta = position - origin;
            float angle = atan(delta.y/delta.x);
            if (delta.x<0) angle+=M_PI;
            float distance = length(delta);
            if (operator[](angle)<=distance) 
                vecs.push_back(position);
        }
    }
};
class Area {
    Vector2 size, origin;
    std::vector<Tectonic*> Tectonics;
    std::vector<Tectonic*> specials;
public:
    Area() {
        srand(clock());
    }
    Area(const int& n, const Vector2& org, const Vector2& s): origin(org), size(s) {
        Tectonics.resize(n);
        float remain = 1;
        for (int i = 0; i<n; i++) {
            float percent = 1.0f*(rand()%70)/100 + 0.25;
            Vector2 subsize =  {1.5f*percent*s.y, percent*s.y};
            Tectonics[i] = new Tectonic(20, subsize);
            Vector2 position;
            percent = 1.0f*(rand()%30)/100;
            percent = percent*remain + 1 - remain;
            remain = (1 - percent)*0.7;
            position.x = (size.x-subsize.x)*percent+subsize.x/2 + origin.x;
            percent = 1.0f*(rand()%100)/100;
            position.y = (size.y-subsize.y)*percent + subsize.y/2 + origin.y;
            Tectonics[i]->setPosition(position);
        }
    }
    Area(const Area&) = delete;
    ~Area() {
        for (auto& Tectonic: Tectonics) delete Tectonic;
        for (auto& Tectonic: specials) delete Tectonic;
        Tectonics.clear();
        specials.clear();
    }
    Area& operator=(const Area&) const = delete;
    void draw(const Vector2& bound, unsigned char** board) const {
        for (auto& Tectonic: Tectonics) Tectonic->draw(bound, board);
    }
};
int main() {
    InitWindow(2500, 1500, "World Render");
    srand(clock());
    Area area(10, {250,250}, {2500, 1000});
    unsigned char** board;
    
    board = new unsigned char*[3000];
    for (int i = 0; i<3000; i++) {
        board[i] = new unsigned char[1500];
        std::memset(board[i], 0, 1500);
    }
    Image image = GenImageColor(3000, 1500, BLACK);
    Color* colors = (Color*)image.data;

    unsigned char max_height = 0;
    float average_height = 0;
    size_t count = 0;
    area.draw({3000, 1500}, board);
    for (int y = 0; y<image.height; y++) {
        for (int x = 0; x<image.width; x++) { 
            int index = y*image.width + x;
            if (board[x][y]) colors[index] = WHITE;
            // unsigned char color = board[x][y]*2;
            // max_height = std::max(max_height, board[x][y]);
            // if (color) {
            //     average_height += board[x][y];
            //     count++;
            // }
            // colors[index] = {color, color, color, 255};
        }
    }
    std::cout << (int)max_height << std::endl;
    std::cout << average_height/count << std::endl;

    for (int i = 0; i<3000; i++) delete[] board[i];
    delete[] board;

    Texture2D Texture2D = LoadTextureFromImage(image);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Texture2D, 0, 0, WHITE);
        EndDrawing();
    }
    UnloadTexture(Texture2D);
    UnloadImage(image);
    CloseWindow();
    return 0;
}

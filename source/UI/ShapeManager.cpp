#include "ShapeManager.h"
#include "Ellipse.h"
#include "Global.h"
#include "Rectangle.h"
namespace MyBase {
    ShapeManager* ShapeManager::Default;
    ShapeManager::ShapeManager() {}
    ShapeManager::~ShapeManager() {
        std::cout<<"Rectangle remains: " << __rectangles.size() << std::endl;
        std::cout<<"RoundedRectangle remains: " << __rrectangles.size() << std::endl;
        std::cout<<"Ellipse remains: " << __ellipses.size() << std::endl;
        std::cout<<"Color remains: " << __colors.size() << std::endl;
    }
    ShapeManager& ShapeManager::getInstance() {
        if (!Default) Default = new ShapeManager();
        return *Default;
    }
    void ShapeManager::close() {
        if (Default) {
            delete Default;
            Default = 0;
        }
    }
    void ShapeManager::createShape(Rectangle& rectangle, const glm::vec2& size) {
        std::pair<float, float> pairs = {size.x, size.y};
        if (__rectangles.find(pairs)==__rectangles.end()) {
            __rectangles[pairs]={new Rectangle(size), 1};
        }
        else __rectangles[pairs].second++;
        rectangle = *__rectangles[pairs].first;
    }
    void ShapeManager::removeShape(Rectangle& rectangle, const glm::vec2& size) {
        std::pair<float, float> pairs = {size.x, size.y};
        if (__rectangles.find(pairs)!=__rectangles.end()) {
            auto& rectange = __rectangles[pairs];
            rectange.second--;
            if (!rectange.second) {
                delete rectange.first;
                __rectangles.erase(pairs);
            }
        }
    }
    void ShapeManager::createShape(Ellipse& ellipse, const glm::vec2& size) {
        std::pair<float, float> pairs = {size.x, size.y};
        if (__ellipses.find(pairs)==__ellipses.end()) {
            __ellipses[pairs]={new Ellipse(size), 1};
        }
        else __ellipses[pairs].second++;
        ellipse = *__ellipses[pairs].first;
    }
    void ShapeManager::removeShape(Ellipse& ellipse, const glm::vec2& size) {
        std::pair<float, float> pairs = {size.x, size.y};
        if (__ellipses.find(pairs)!=__ellipses.end()) {
            auto& ellipse = __ellipses[pairs];
            ellipse.second--;
            if (!ellipse.second) {
                delete ellipse.first;
                __ellipses.erase(pairs);
            }
        }
    }
    void ShapeManager::createShape(RoundedRectangle& rectangle, const glm::vec2& size, const float& radius) {
        std::tuple<float, float, float> trips = {size.x, size.y, radius};
        if (__rrectangles.find(trips)==__rrectangles.end()) {
            __rrectangles[trips]={new RoundedRectangle(size, radius), 1};
        }
        else __rrectangles[trips].second++;
        rectangle = *__rrectangles[trips].first;
    }
    void ShapeManager::removeShape(RoundedRectangle& rectangle, const glm::vec2& size, const float& radius) {
        std::tuple<float, float, float> trips = {size.x, size.y, radius};
        if (__rrectangles.find(trips)!=__rrectangles.end()) {
            auto& rectange = __rrectangles[trips];
            rectange.second--;
            if (!rectange.second) {
                delete rectange.first;
                __rrectangles.erase(trips);
            }
        }
    }
    const GLuint& ShapeManager::createColor(const Color& color) {
        int code = *(int*)&color;
        if (__colors.find(code)==__colors.end()) {
            GLuint COLOR;
            glm::vec4 colorData = color.getColor();
            glGenBuffers(1, &COLOR);
            glBindBuffer(GL_UNIFORM_BUFFER, COLOR);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &colorData, GL_STATIC_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
            __colors[code] = {COLOR, 1};
        }
        else __colors[code].second++;
        return __colors[code].first;
    }
    void ShapeManager::removeColor(const Color& color) {
        int code = *(int*)&color;
        if (__colors.find(code)!=__colors.end()) {
            auto& node = __colors[code];
            node.second--;
            if (!node.second) {
                glDeleteBuffers(1, &node.first);
                __colors.erase(code);
            }
        }
    }
}
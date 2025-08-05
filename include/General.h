#ifndef GENERAL_H
#define GENERAL_H
#include "Color.h"
#include "Ray.h"
#include "Rect3D.h"
#include "Texture.h"

bool isIntersect(const MyBase3D::Ray3f& ray, const MyBase3D::Rect3f& rectangle);

std::size_t GetTime();

std::queue<glm::ivec3> rasterize(const glm::vec3& a, const glm::vec3& b, const float& stride = 0.1);

std::ostream& operator<<(std::ostream& cout, const glm::vec3& pos);
std::tuple<int,int,int> to_tuple(const glm::ivec3& vec);


namespace MyBase {
    glm::vec2 getWindowSize();
}
namespace MyCraft {
    void DrawMargin(const glm::mat4x3& box, const glm::vec3& color, const float& linewidth = 2);
}
namespace MyBase {
    bool isFolder(const std::string& src);
    void CreateFolder(const std::string& src);
    void DeleteFile(const std::string& src);
    void DeleteFolder(const std::string& src);
    void DrawTexture(GLuint texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& subposition, const glm::vec2& subsize);
    void DrawRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color);
    Texture LoadTexture(const std::string& src, const bool& isReverse = true);
}
#endif
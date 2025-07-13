#include "General.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include "glm/geometric.hpp"

float abs(const glm::vec2& v) {
    return sqrt(v.x*v.x + v.y*v.y);
}

float operator*(const glm::vec3& a, const glm::vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
glm::vec2 getMousePosition(GLFWwindow* window) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {x, y};
}
std::size_t GetTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
glm::vec3 det(const glm::vec3& a, const glm::vec3& b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
std::ostream& operator<<(std::ostream& cout, const glm::vec3& pos) {
    cout << "(" << pos.x << ", "<<pos.y << ", " << pos.z << ") ";
    return cout;
}
std::queue<glm::vec3> rasterize(const glm::vec3& a, const glm::vec3& b) {
    std::queue<glm::vec3> ans;
    glm::vec3 delta = (b-a);
    glm::vec3 curr = a;
    ans.push({floor(a.x), floor(a.y), floor(a.z)});
    int n = glm::length(delta)/0.1;
    if (!n) return ans;
    delta = glm::normalize(delta)*0.1f;
    for (int i = 0; i<n; i++) {
        curr+=delta;
        glm::vec3 tmp(floor(curr.x), floor(curr.y), floor(curr.z));
        if (tmp!=ans.back()) ans.push(tmp);
    }
    return ans;
}
std::queue<glm::vec3> rasterize(const glm::mat3& rec) {
    return {};
}
namespace MyBase {
    glm::vec2 getWindowSize() {
        #ifdef __linux__
        XL::Display* display = XL::XOpenDisplay(NULL);
        if (!display) {
            std::cerr << "Cannot open display\n";
            exit(0);
        }
        using namespace XL;
        XL::Screen* screen = ScreenOfDisplay(display, 0);
        float x = screen->width, y = screen->height;
        XCloseDisplay(display);
        return {x, y};
        #endif

        #ifdef _WIN32
        SetProcessDPIAware();
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        std::cout << width << " " << height << std::endl;
        return {width, height};
        #endif
    }
}
namespace MyCraft {
    void DrawMargin(const glm::mat4x3& mat, const glm::vec3& color, const float& linewidth) {
        glm::vec3 shape[8] = {mat[0], mat[0]+mat[1], mat[0] + mat[1]+ mat[2], mat[0] +mat[2]};
        for (int i = 4; i<8; i++) shape[i] = shape[i-4]+mat[3];

        GLuint VAO, VBO, COLOR;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, 96, &shape, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);

        glGenBuffers(1, &COLOR);
        glBindBuffer(GL_UNIFORM_BUFFER, COLOR);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), &color, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, COLOR);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());
        glUseProgram(MyBase3D::ShaderStorage::Default->GetDefaultShader());
        
        glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &COLOR);
        glDeleteVertexArrays(1, &VAO);
    }
}
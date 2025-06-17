#include "General.h"
#include "Block.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"

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
    glm::vec3 delta = (b-a)/2.f;
    glm::vec3 i = a;
    ans.push({floor(a.x), floor(a.y), floor(a.z)});
    glm::vec3 p(floor(b.x), floor(b.y), floor(b.z));
    while (ans.back()!=p) {
        i+=delta;
        glm::vec3 tmp(floor(i.x), floor(i.y), floor(i.z));
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
        Display* display = XOpenDisplay(NULL);
        if (!display) {
            std::cerr << "Cannot open display\n";
            exit(0);
        }
        Screen* screen = ScreenOfDisplay(display, 0);
        float x = screen->width, y = screen->height;
        XCloseDisplay(display);
        return {x, y};
        #endif

        #ifdef _WIN32
        return {1920, 1080};
        #endif
    }
}
namespace MyCraft {
    void DrawCube(const unsigned char& blockType, const glm::vec3& position) {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetCubeShader());
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getBlockSet());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, MyCraft::BlockCatogary::Default->getTexCoord());
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getImageBlockIndices());

        GLuint originPoint;
        
        glGenBuffers(1, &originPoint);
        glBindBuffer(GL_UNIFORM_BUFFER, originPoint);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*3, &position[0], GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MyCraft::BlockCatogary::Default->getBlock(blockType));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &originPoint);
    }
    void DrawMargin(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& color, const float& linewidth) {
        glUseProgram(MyBase3D::ShaderStorage::Default->GetMarginShader());
        glLineWidth(linewidth);
        GLuint VAO, originPoint;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::Default->getBlockSet());
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &originPoint);
        glBindBuffer(GL_UNIFORM_BUFFER, originPoint);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), &position, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, originPoint);

        float buffer[8] = {
            scale.x, scale.y, scale.z, 0,
            color.r, color.g, color.b, 1
        };
        GLuint margin;
        glGenBuffers(1, &margin);
        glBindBuffer(GL_UNIFORM_BUFFER, margin);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*8, buffer, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, margin);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::Default->getMarginBlockIndices());
        glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &margin);
        glDeleteBuffers(1, &originPoint);
    }
    void DrawMargin(const glm::mat4& state, const glm::vec3& scale, const glm::vec3& color, const float& linewidth) {
    }
}
#include "General.h"
#include "Global.h"
#include "PointSet.h"
#include "ShaderStorage.h"
#include <filesystem>
#include <stdexcept>


std::size_t GetTime() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

std::ostream& operator<<(std::ostream& cout, const glm::vec3& pos) {
    cout << "(" << pos.x << ", "<<pos.y << ", " << pos.z << ") ";
    return cout;
}
std::queue<glm::ivec3> rasterize(const glm::vec3& a, const glm::vec3& b) {
    std::queue<glm::ivec3> ans;
    glm::vec3 delta = (b-a);
    glm::vec3 curr = a;
    ans.push({floor(a.x), floor(a.y), floor(a.z)});
    int n = glm::length(delta)/0.1;
    if (!n) return ans;
    delta = glm::normalize(delta)*0.1f;
    for (int i = 0; i<n; i++) {
        curr+=delta;
        glm::ivec3 tmp(floor(curr.x), floor(curr.y), floor(curr.z));
        if (tmp!=ans.back()) ans.push(tmp);
    }
    return ans;
}

std::tuple<int,int,int> to_tuple(const glm::ivec3& vec) {
    return {vec.x, vec.y, vec.z};
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
    void DrawTexture(GLuint texture, const glm::vec2& position, const glm::vec2& size, const glm::vec2& subposition, const glm::vec2& subsize) {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getImage2DShader());
        GLuint POS, SIZE, SPOS, SSIZE, VAO;

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glGenBuffers(1, &POS);
        glBindBuffer(GL_UNIFORM_BUFFER, POS);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, &position, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, POS);

        glGenBuffers(1, &SIZE);
        glBindBuffer(GL_UNIFORM_BUFFER, SIZE);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, &size, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, SIZE);

        glGenBuffers(1, &SPOS);
        glBindBuffer(GL_UNIFORM_BUFFER, SPOS);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, &subposition, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, SPOS);


        glGenBuffers(1, &SSIZE);
        glBindBuffer(GL_UNIFORM_BUFFER, SSIZE);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*2, &subsize, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 4, SSIZE);

        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getRectangle2DSet());
        glEnableVertexArrayAttrib(VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getRectangle2DUV());
        glEnableVertexArrayAttrib(VAO, 1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &POS);
        glDeleteBuffers(1, &SIZE);
        glDeleteBuffers(1, &SPOS);
        glDeleteBuffers(1, &SSIZE);
    }
    void DrawRectangle(const glm::vec2& position, const glm::vec2& size, const Color& c) {
        glUseProgram(MyBase3D::ShaderStorage::getInstance().getPoint2DShader());
        GLuint VAO, VBO, ORI, COLOR;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glm::vec2 positions[4] = {position, position+glm::vec2(size.x,0), position + size, position + glm::vec2(0, size.y)};
        glm::vec2 origin = {0,0};
        glm::vec4 color = c.getColor();

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*4, &positions[0], GL_STATIC_DRAW);
        glEnableVertexArrayAttrib(VAO, 0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

        glGenBuffers(1, &ORI);
        glBindBuffer(GL_UNIFORM_BUFFER, ORI);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &origin, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, COLOR);

        glGenBuffers(1, &COLOR);
        glBindBuffer(GL_UNIFORM_BUFFER, COLOR);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), &color, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, COLOR);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &ORI);
        glDeleteBuffers(1, &COLOR);
    }
    void CreateFolder(const std::string& src) {
        if (!std::filesystem::create_directory(src)) {
            throw std::runtime_error("Failed to create folder: " + src);
        }
    }
    void DeleteFolder(const std::string& src) {
        if (!std::filesystem::remove_all(src)) {
            throw std::runtime_error("Failed to delete folder: " + src);
        }
    }
    void DeleteFile(const std::string& src) {
        remove(src.c_str());
    }
    GLuint LoadTexture(const std::string& src) {
        GLuint texture;
        int width, height, nrChannels;
        unsigned char* data = stbi_load(src.c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            throw std::runtime_error("Failed to load texture: " + src);
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return texture;
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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MyBase3D::PointSet::getInstance().getMarginBlockIndices());
        glUseProgram(MyBase3D::ShaderStorage::getInstance().GetDefaultShader());
        
        glDrawElements(GL_LINE_STRIP, 16, GL_UNSIGNED_INT, 0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &COLOR);
        glDeleteVertexArrays(1, &VAO);
    }
}
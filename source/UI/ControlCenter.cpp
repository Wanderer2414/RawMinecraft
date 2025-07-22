#include "ControlCenter.h" 
#include "Block.h"
#include "General.h"
#include "Global.h"
#include <ctime>
namespace MyBase {
    ControlCenter::ControlCenter(const float& width, const float& height, const std::string& program):
         __windowSize(width, height), __windowHalfSize(__windowSize/2.f), __programName(program), __fpsInterval(100), __majorVerson(3), __minorVerson(3),
         __scrollPosition(0, 0), __clock(clock()) {

    }
    ControlCenter::~ControlCenter() {
    }

    float ControlCenter::getWindowRatio() const {
        return __windowSize.x/__windowSize.y;
    }
    const glm::vec2& ControlCenter::getWindowSize() const {
        return __windowSize;
    }
    const glm::vec2& ControlCenter::getWindowHalf() const {
        return __windowHalfSize;    
    }
    void scroll_callback(GLFWwindow* window, double x, double y) {
        ControlCenter::Default->__clock = clock();
        ControlCenter::Default->__scrollPosition = {x,y};
    }
    glm::vec2 ControlCenter::getScroll() const {
        if (clock()-__clock>CLOCKS_PER_SEC*0.001) return {0,0};
        return __scrollPosition;
    }
    GLFWwindow* ControlCenter::InitWindow() {
        if (!glfwInit()) {
            std::cout << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, __majorVerson);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, __minorVerson);
        GLFWwindow* window = glfwCreateWindow(__windowSize.x, __windowSize.y, __programName.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(__fpsInterval);
        if (!window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
        else {
            std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
        }
        glfwSetScrollCallback(window, scroll_callback);

        glGenTextures(1, &__screenTexture);
        glBindTexture(GL_TEXTURE_2D, __screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, __windowSize.x, __windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);    

        glGenFramebuffers(1, &__screenBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, __screenBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, __screenTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return window;
    }
    
    void ControlCenter::CloseWindow() {
        glDeleteTextures(1, &__screenTexture);
        glDeleteFramebuffers(1, &__screenBuffer);
        glfwTerminate();
    }
    void ControlCenter::enable3DMode() {
        glEnable(GL_DEPTH_TEST);
    }
    void ControlCenter::disable3DMode() {
        glDisable(GL_DEPTH_TEST);
    }
    void ControlCenter::OpenGLrequire(const unsigned char& majorVersion, const unsigned char& minorVerson) {
        __majorVerson = majorVersion;
        __minorVerson = minorVerson;
    }
    void ControlCenter::EnableResizable() {
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    }
    void ControlCenter::DisableResizable() {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    }
    void ControlCenter::EnableTransparent() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    void ControlCenter::DisableTransparent() {
        glDisable(GL_BLEND);
    }
    void ControlCenter::LimitFPS(const unsigned int& interval) {
        __fpsInterval = interval;
    }
    void ControlCenter::enableScissorMode(const glm::vec2& position, const glm::vec2& size) const {
        glEnable(GL_SCISSOR_TEST);
        glScissor((position.x+1)*__windowHalfSize.x, (position.y+1)*__windowHalfSize.y, size.x*__windowHalfSize.x, size.y*__windowHalfSize.y);
    }
    void ControlCenter::disableScissorMode() const {
        glDisable(GL_SCISSOR_TEST);
    }

    void ControlCenter::BindSubScreen() const{
        glBindFramebuffer(GL_FRAMEBUFFER, __screenBuffer);
    }
    void ControlCenter::UnbindSubScreen() const{
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void ControlCenter::DrawSavedScreen() const{
        MyCraft::DrawTexture(__screenTexture, {-1,-1}, {2, 2}, {0,0}, {1,1});
    }
    glm::vec2 ControlCenter::getCursorPos(GLFWwindow* window) const {
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 ans(x - __windowHalfSize.x,__windowHalfSize.y - y);
        ans/=__windowHalfSize;
        return ans;
    }
}
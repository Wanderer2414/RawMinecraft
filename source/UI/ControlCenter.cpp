#include "ControlCenter.h" 
#include "General.h"
#include "Global.h"

namespace MyBase {

    ControlCenter* ControlCenter::Default = 0;
    ControlCenter::ControlCenter(): __homeScreen(0),
         __fpsInterval(100), __majorVerson(3), __minorVerson(3), __clickCount(0),
         __scrollPosition(0, 0), __clock(clock()), __charInput(-1), __isKeyPressed(false), __isMaximize(false) {

    }
    ControlCenter::~ControlCenter() {}
    ControlCenter& ControlCenter::getInstance() {
        if (!Default) {
            Default = new ControlCenter();
        }
        return *Default;
    }
    void ControlCenter::close() {
        if (Default) delete Default;
        Default = 0;
    }
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            int c = key - 'A' + 'a';
            if (!(mods & GLFW_MOD_ALT) && !(mods & GLFW_MOD_CONTROL)) {
                if (c>='a' && c<='z') {
                    if (((mods & GLFW_MOD_SHIFT) && !(mods & GLFW_MOD_CAPS_LOCK)) || 
                        (!(mods & GLFW_MOD_SHIFT) && (mods & GLFW_MOD_CAPS_LOCK)))
                            c += 'A'-'a';
                    ControlCenter::getInstance().__charInput = c;
                }
                else if (key>='0' && key<='9') {
                    ControlCenter::getInstance().__charInput = key;
                }
                else if (key == GLFW_KEY_BACKSPACE) {
                    ControlCenter::getInstance().__charInput='\b';
                }
                else if (key == GLFW_KEY_SPACE) {
                    ControlCenter::getInstance().__charInput=' ';
                }
            }
            ControlCenter::getInstance().__isKeyPressed = true;
        }
        else if (action == GLFW_RELEASE) {
            ControlCenter::getInstance().__charInput = -1;
        }
    }

    void mouse_callback(GLFWwindow* window, int buttons, int actions, int mods) {
        if (actions) {
            ControlCenter::Default->__isMouseClicked = true;
            if (ControlCenter::Default->__clickCount) ControlCenter::Default->__isDoubleClick = true;
        }
        else ControlCenter::Default->__clickCount = 100;
    }

    void scroll_callback(GLFWwindow* window, double x, double y) {
        ControlCenter::getInstance().__scrollPosition = {x,y};
    }

    bool ControlCenter::IsKeyPressed() const {
        return __isKeyPressed;
    }
    bool ControlCenter::IsMouseClicked() const {
        return __isMouseClicked;
    }
    bool ControlCenter::IsDoubleClicked() const {
        return __isDoubleClick;
    }
    char ControlCenter::GetCharInput() const {
        return __charInput;
    }
    float ControlCenter::GetWindowRatio() const {
        return __windowSize.x/__windowSize.y;
    }
    const glm::vec2& ControlCenter::getWindowSize() const {
        return __windowSize;
    }
    const glm::vec2& ControlCenter::getWindowHalf() const {
        return __windowHalfSize;    
    }
    void ControlCenter::Reset() {
        __scrollPosition = {0,0};
        __isKeyPressed = __isMouseClicked = __isDoubleClick = false;
        if (__clickCount) __clickCount--;
        __charInput = 0;
    }
    glm::vec2 ControlCenter::getScroll() const {
        return __scrollPosition;
    }
    GLFWwindow* ControlCenter::InitWindow(const float& width, const float& height, const std::string& program) {
        __windowSize = {width, height};
        __windowHalfSize = __windowSize/2.f;
        __programName = program;

        if (!glfwInit()) {
            throw std::runtime_error( "Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, __majorVerson);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, __minorVerson);
        glfwWindowHint(GLFW_MAXIMIZED, __isMaximize);
        glfwWindowHint(GLFW_DECORATED, !__isFullScreen);
        GLFWwindow* window = glfwCreateWindow(__windowSize.x, __windowSize.y, __programName.c_str(), glfwGetPrimaryMonitor(), nullptr);
        if (!__homeScreen) __homeScreen = window;
        glfwMakeContextCurrent(window);
        glfwSwapInterval(__fpsInterval);
        if (!window) throw std::runtime_error("Failed to create GLFW window");
        std::cout << width << " " << height << std::endl;
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) throw std::runtime_error("Failed to initialize GLAD");
        else std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_callback);

        glGenTextures(1, &__screenTexture);
        glBindTexture(GL_TEXTURE_2D, __screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, __windowSize.x, __windowSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);    

        glGenRenderbuffers(1, &__renderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, __renderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

        glGenFramebuffers(1, &__screenBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, __screenBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, __screenTexture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, __renderBuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return window;
    }
    GLFWwindow* ControlCenter::getHomeScreeen() const {
        return __homeScreen;
    }
    
    void ControlCenter::EnableFullScreenMode() {
        __isFullScreen = true;
    }
    void ControlCenter::DisableFullScreenMode() {
        __isFullScreen = false;
    }
    void ControlCenter::CloseWindow() {
        glDeleteTextures(1, &__screenTexture);
        glDeleteFramebuffers(1, &__screenBuffer);
        glDeleteRenderbuffers(1, &__renderBuffer);
        glfwTerminate();
    }
    void ControlCenter::Enable3DMode() {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
    }
    void ControlCenter::Disable3DMode() {
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
    void ControlCenter::EnableScissorMode(const glm::vec2& position, const glm::vec2& size) const {
        glEnable(GL_SCISSOR_TEST);
        glScissor((position.x+1)*__windowHalfSize.x, (position.y+1)*__windowHalfSize.y, size.x*__windowHalfSize.x, size.y*__windowHalfSize.y);
    }
    void ControlCenter::DisableScissorMode() const {
        glDisable(GL_SCISSOR_TEST);
    }
    void ControlCenter::Maximize() {
        __isMaximize = true;
    }

    void ControlCenter::BindSubScreen() const{
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, __screenBuffer);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        glBlitFramebuffer(0, 0, __windowSize.x, __windowSize.y,
                        0, 0, __windowSize.x, __windowSize.y,
                        GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void ControlCenter::DrawSavedScreen() const{
        DrawTexture(__screenTexture, {-1,-1}, {2, 2}, {0,0}, {1,1});
    }
    void ControlCenter::EnableMouse(GLFWwindow* window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    void ControlCenter::CenteringMouse(GLFWwindow* window) {
        glfwSetCursorPos(window, MyBase::ControlCenter::getInstance().getWindowHalf().x, MyBase::ControlCenter::getInstance().getWindowHalf().y);
    }
    void ControlCenter::DisableMouse(GLFWwindow* window) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    glm::vec2 ControlCenter::getCursorPos(GLFWwindow* window) const {
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 ans(x - __windowHalfSize.x,__windowHalfSize.y - y);
        ans/=__windowHalfSize;
        return ans;
    }
}
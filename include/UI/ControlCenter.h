#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H
#include "Global.h"
#include "Rectangle.h"
namespace MyBase {
    class ControlCenter {
        public:
            GLFWwindow* InitWindow(const float& width, const float& height, const std::string& program);
            float GetWindowRatio() const;
            void CloseWindow();
            void OpenGLrequire(const unsigned char& majorVersion, const unsigned char& minorVerson);

            static void EnableResizable();
            static void DisableResizable();

            static void EnableTransparent();
            static void DisableTransparent();

            void LimitFPS(const unsigned int& interval);

            static void Enable3DMode();
            static void Disable3DMode();

            void EnableScissorMode(const glm::vec2& position, const glm::vec2& size) const;
            void DisableScissorMode()   const;

            static void EnableMouse(GLFWwindow*);
            static void CenteringMouse(GLFWwindow*);
            static void DisableMouse(GLFWwindow*);

            void Maximize();
            
            void BindSubScreen()        const;
            void DrawSavedScreen()      const;
            
            GLFWwindow* getHomeScreeen() const;

            void Reset();
            bool IsMouseClicked() const;
            bool IsDoubleClicked() const;
            bool IsKeyPressed() const;
            char GetCharInput() const;
            glm::vec2 getCursorPos(GLFWwindow* window) const;
            glm::vec2 getScroll() const;
            const glm::vec2& getWindowSize() const;
            const glm::vec2& getWindowHalf() const;

            friend void mouse_callback(GLFWwindow*, int, int, int);
            friend void scroll_callback(GLFWwindow*, double, double);
            friend void key_callback(GLFWwindow*, int, int, int, int);

            static ControlCenter& getInstance();
            static void close();
        private: 
            bool            __isKeyPressed, __isMouseClicked, __isDoubleClick;
            unsigned char   __majorVerson, __minorVerson, __clickCount, __isMaximize;
            char            __charInput;
            float           __fpsInterval;
            GLuint          __screenTexture, __screenBuffer, __renderBuffer;
            size_t          __clock;
            glm::vec2       __scrollPosition;
            glm::vec2       __windowSize, __windowHalfSize;
            std::string     __programName;
            
            GLFWwindow*     __homeScreen;
            ControlCenter();
            ~ControlCenter();
            static ControlCenter* Default;
    };
};
#endif
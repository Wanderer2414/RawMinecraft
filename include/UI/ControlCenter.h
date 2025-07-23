#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H
#include "Global.h"
namespace MyBase {
    class ControlCenter {
        public:
            GLFWwindow* InitWindow(const float& width, const float& height, const std::string& program);
            float GetWindowRatio() const;
            void CloseWindow();
            void OpenGLrequire(const unsigned char& majorVersion, const unsigned char& minorVerson);

            void EnableResizable();
            void DisableResizable();

            void EnableTransparent();
            void DisableTransparent();

            void LimitFPS(const unsigned int& interval);

            void Enable3DMode();
            void Disable3DMode();

            void EnableScissorMode(const glm::vec2& position, const glm::vec2& size) const;
            void DisableScissorMode()   const;

            void EnableMouse(GLFWwindow*);
            void DisableMouse(GLFWwindow*);
            
            void BindSubScreen()        const;
            void UnbindSubScreen()      const;
            void DrawSavedScreen()      const;

            void Reset();
            bool IsKeyPressed() const;
            char GetCharInput() const;
            glm::vec2 getCursorPos(GLFWwindow* window) const;
            glm::vec2 getScroll() const;
            const glm::vec2& getWindowSize() const;
            const glm::vec2& getWindowHalf() const;
            friend void scroll_callback(GLFWwindow*, double, double);
            friend void key_callback(GLFWwindow*, int, int, int, int);

            static ControlCenter& getInstance();
            static void close();
        private: 
            bool            __isKeyPressed;
            unsigned char   __majorVerson, __minorVerson;
            char            __charInput;
            float           __fpsInterval;
            GLuint          __screenTexture, __screenBuffer, __renderBuffer;
            size_t          __clock;
            glm::vec2       __scrollPosition;
            glm::vec2       __windowSize, __windowHalfSize;
            std::string     __programName;
            ControlCenter();
            ~ControlCenter();
            static ControlCenter* Default;
    };
};
#endif
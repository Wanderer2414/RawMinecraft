#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H
#include "Global.h"
namespace MyBase {
    class ControlCenter {
        public:
            ControlCenter(const float& width, const float& height, const std::string& program);
            ~ControlCenter();
            static ControlCenter* Default;
            GLFWwindow* InitWindow();
            float getWindowRatio() const;
            void CloseWindow();
            void OpenGLrequire(const unsigned char& majorVersion, const unsigned char& minorVerson);
            void EnableResizable();
            void DisableResizable();
            void EnableTransparent();
            void DisableTransparent();
            void LimitFPS(const unsigned int& interval);
            void enable3DMode();
            void disable3DMode();
            void enableScissorMode(const glm::vec2& position, const glm::vec2& size) const;
            void disableScissorMode()   const;
            void BindSubScreen()        const;
            void UnbindSubScreen()      const;
            void DrawSavedScreen()      const;
            glm::vec2 getCursorPos(GLFWwindow* window) const;
            glm::vec2 getScroll() const;
            const glm::vec2& getWindowSize() const;
            const glm::vec2& getWindowHalf() const;
            friend void scroll_callback(GLFWwindow*, double, double);
        private: 
            unsigned char   __majorVerson, __minorVerson;
            float           __fpsInterval;
            GLuint          __screenTexture, __screenBuffer;
            size_t          __clock;
            glm::vec2       __scrollPosition;
            glm::vec2       __windowSize, __windowHalfSize;
            std::string     __programName;
    };
};
#endif
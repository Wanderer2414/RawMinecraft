#ifndef TOOL_BAR_H
#define TOOL_BAR_H
#include "Controller2D.h"
#include "Texture.h"
namespace MyCraft {
    class ToolBar: public MyBase::Controller2D, private MyBase::TextureContainer {
    public:
        ToolBar();
        ~ToolBar();
        ToolBar(const ToolBar&) = delete;
        ToolBar& operator=(const ToolBar&) const = delete; 
        glm::vec2 getPosition() const override;
        glm::vec2 getSize() const override;
    protected:
    private:
        GLuint __texture;
        glm::vec2 __size, __position;

        bool contains(const glm::vec2& position) const override;
        void glDraw() const override;
    };
}
#endif
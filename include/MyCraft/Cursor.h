#ifndef CURSOR_H
#define CURSOR_H
#include "Controller2D.h"
namespace MyCraft {
    class Cursor: public MyBase::Controller2D {
    public:
        Cursor();
        ~Cursor();
        Cursor(const Cursor&) = delete;
        Cursor& operator=(const Cursor&) const = delete; 

        glm::vec2       getPosition() const override,
                        getSize() const override;
        bool contains(const glm::vec2& pos) const override;
    protected:
    private:
        void glDrawTransparent() const override;
    };
}
#endif
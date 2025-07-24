#ifndef FORM_H
#define FORM_H

#include "Clock.h"
#include "Color.h"
#include "Container2D.h"
#include "Global.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyBase {

    class Form: public Container2D, private ShapeContainer {
    public:
        Form(const int& index);
        ~Form();
        virtual bool    contains(const glm::vec2& position) const override;
        virtual void    setSensitiveTime(const size_t& milisecond);
        int             run(GLFWwindow* window);
        void            close();

        glm::vec2       getPosition()   const override, 
                        getSize()       const override;
        void            setBackgroundColor(const Color& color);
        void            pauseScreen(GLFWwindow* window);
    protected:
        int             getReturnValue() const;
        int             getFormIndex();
        void            setReturnValue(const int& returnValue);
    private:
        bool            __isOpen;
        int             __formIndex,
                        __returnValue;
        Color           __backgroundColor;
        Clock           __sensitiveClock;
        Rectangle       __pauseScreen;
    };
    
};
#endif
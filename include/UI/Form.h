#ifndef FORM_H
#define FORM_H

#include "Clock.h"
#include "Color.h"
#include "Container2D.h"
#include "Global.h"
#include "Rectangle.h"
#include "Shape.h"
#include "Sound.h"
#include "Slider.h"

namespace MyBase {

    class Form: public Container2D {
    public:
        Form(const int& index);
        ~Form();
        virtual bool    contains(const glm::vec2& position) const override;
        int             run(GLFWwindow* window);
        void            close();

        glm::vec2       getPosition()   const override, 
                        getSize()       const override;
        void            setBackgroundColor(const Color& color);
    protected:
        int             getReturnValue() const;
        int             getFormIndex();
        void            setReturnValue(const int& returnValue);
    private:
        bool            __isOpen;
        int             __formIndex,
                        __returnValue;
        Color           __backgroundColor;
    };
    
};
#endif
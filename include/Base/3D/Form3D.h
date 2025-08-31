#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Clock.h"
#include "Color.h"
#include "Container2D.h"
#include "Container3D.h"
#include "Global.h"
#include "MessageBox.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyBase3D {

    class Form3D: public MyBase::Container2D, public Container3D {
    public:
        Form3D(const int& index);
        ~Form3D();
        virtual bool    contains(const glm::vec2& position) const override;
        int             run(GLFWwindow* window);

        glm::vec2       getPosition()   const override, 
                        getSize()       const override;
        void            setBackgroundColor(const MyBase::Color& color);
        using MyBase::Container2D::insert;
        using MyBase::Container2D::insertPermanent;
        using Container3D::insert;
        using MyBase::Container2D::erase;
        using Container3D::erase;
    protected:
        int                     getReturnValue() const;
        int                     getFormIndex();
        float                   getAverageFps() const, getCurrentFps() const;
        void                    setReturnValue(const int& returnValue);
        void                    close();
        virtual bool            catchEvent(GLFWwindow* window)              override;
        virtual bool            handle(GLFWwindow* window)                  override;
        virtual void            glDraw() const override;
        virtual void            glDrawTransparent() const override;
        virtual void            __open(GLFWwindow* window), __close(GLFWwindow* window);
    private:
        bool                    __isOpen;
        int                     __formIndex,
                                __returnValue;
        size_t                  __startClock, __frameCount, __currentClock, __deltaClock;
        MyBase::Color           __backgroundColor;
    };
    
};
#endif
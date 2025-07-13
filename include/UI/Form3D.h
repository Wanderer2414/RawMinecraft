#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Clock.h"
#include "Color.h"
#include "Container2D.h"
#include "Container3D.h"
#include "Global.h"
#include "Rectangle.h"
#include "Shape.h"
namespace MyBase3D {

    class Form3D: public MyBase::Container2D, public Container3D, private MyBase::ShapeContainer {
    public:
        Form3D(const int& index);
        ~Form3D();
        virtual bool    contains(const glm::vec2& position) const override;
        virtual void    setSensitiveTime(const size_t& milisecond);
        int             run(GLFWwindow* window);

        glm::vec2       getPosition()   const override, 
                        getSize()       const override;
        void            setBackgroundColor(const MyBase::Color& color);
        void            pauseScreen(GLFWwindow* window);
        using MyBase::Container2D::insert;
        using MyBase::Container2D::insertPermanent;
        using Container3D::insert;
        using MyBase::Container2D::erase;
        using MyBase::Container2D::erasePermanent;
        using Container3D::erase;
    protected:
        int                     getReturnForm() const;
        int                     getFormIndex();
        float                   getMaxFps() const;
        void                    setReturnForm(const int& returnValue);
        virtual bool            sensitiveHandle(GLFWwindow* window)         override;
        virtual bool            catchEvent(GLFWwindow* window)              override;
        virtual bool            handle(GLFWwindow* window)                  override;
        virtual void            glDraw() const override;
        Camera                  camera;
    private:
        MyBase::Rectangle       __pauseScreen;
        size_t                  __startClock, __frameCount;
        int                     __formIndex,
                                __returnValue;
        MyBase::Color           __backgroundColor;
        MyBase::Clock           __sensitiveClock;
    };
    
};
#endif
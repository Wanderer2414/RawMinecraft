#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Container.h"
#include "Container3D.h"
#include "Global.h"
namespace MyBase3D {
    class Form3D: public MyBase::Container, public Container3D {
        public:
            Form3D(const int& index);
            ~Form3D();                                     
            virtual bool            contains(const Ray3f& sight) const override;
            virtual int             run(GLFWwindow* window);
            using Container3D::insert;
            using Container3D::erase;
            using Container::insert;
            using Container::erase;
        protected:
            virtual bool            handle(GLFWwindow* window) override;
            virtual void            glDraw() const override;
            int             _formIndex, 
                            _returnValue;
            Camera          _camera;
        };       
}
#endif
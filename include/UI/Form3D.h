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
            virtual void            insert(Controller* controller, const int& layer = 0)    override,
                                    erase(Controller* controller)                           override,
                                    insert(Controller3D* controller, const int& layer = 0)  override,
                                    erase(Controller3D* controller)                         override;
            virtual bool            contains(const Ray3f& sight) const override;
            virtual int             run(GLFWwindow* window);
        protected:
            virtual bool            handle(GLFWwindow* window) override;
            virtual void            glDraw() const override;
            int             _formIndex, 
                            _returnValue;
            Camera          _camera;
        };       
}
#endif
#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Clock.h"
#include "Container.h"
#include "Global.h"
namespace MyBase3D {
    class Form3D: public MyBase::Container {
        public:
            Form3D(const int& index);
            ~Form3D();                                     
            virtual bool            contains(const glm::vec2& pos) const override;
            virtual int             run(GLFWwindow* window);
            virtual void            setSensitiveTime(const size_t& milisecond);
            float                   getMaxFps() const;
            virtual glm::vec2       getPosition() const                  override,
                                    getSize() const                      override;
            using Container::insert;
            using Container::erase;
        protected:
            virtual bool            sensitiveHandle(GLFWwindow* window) override;
            virtual bool            catchEvent(GLFWwindow* window) override;
            virtual bool            handle(GLFWwindow* window) override;
            int                     getReturnForm() const;
            int                     getFormIndex();
            void                    setReturnForm(const int& returnValue);
            virtual void            glDraw() const override;
            Camera          _camera;
        private:
            int             __formIndex, 
                            __returnValue;
            size_t          __startTime, __frameCount;
            MyBase::Clock   __sensitiveClock;
        };       
}
#endif
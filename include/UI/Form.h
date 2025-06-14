#ifndef FORM_H
#define FORM_H
#include "Clock.h"
#include "Container.h"
#include "Global.h"
namespace MyBase {

    class Form: public Container {
    public:
        Form(const int& index);
        ~Form();
        virtual bool    contains(const glm::vec2& position) const override;
        virtual void    setSensitiveTime(const size_t& milisecond);
        virtual int     run(GLFWwindow* window);
    protected:
        int             getReturnForm() const;
        int             getFormIndex();
        void            setReturnForm(const int& returnValue);
    private:
        int             __formIndex,
                        __returnValue;
        Clock           __sensitiveClock;
    };
    
};
#endif
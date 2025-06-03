#ifndef FORM_H
#define FORM_H
#include "Container.h"
#include "Global.h"
namespace MyBase {

    class Form: public Container {
    public:
        Form(const int& index);
        ~Form();
        virtual bool    contains(const glm::vec2& position) const override;
        virtual int     run(GLFWwindow* window);
    protected:
        virtual void    glDraw() const override;
        int             _formIndex, 
                        _returnValue;
    };
    
};
#endif
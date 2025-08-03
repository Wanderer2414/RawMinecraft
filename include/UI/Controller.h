#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Global.h"
namespace MyBase {
    class Container;

    class Controller {
    public:
        virtual void    update();      

        friend Container;
    protected:
        Controller();
        ~Controller();
        virtual bool    catchEvent(GLFWwindow* window);
        virtual bool    handle(GLFWwindow* window);
        virtual void    reset();
        virtual void    glDraw() const;
        virtual void    glDrawTransparent() const;
    private:
    };
};
#endif
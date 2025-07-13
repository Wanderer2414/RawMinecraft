#ifndef CONTROLLER_3D_H
#define CONTROLLER_3D_H
#include "Controller.h"
#include "Global.h"
namespace MyBase3D {
    class Controller3D: public MyBase::Controller {
    public:
        Controller3D();
        ~Controller3D();
        bool isVisible()         const;
        void setVisible(const bool& visible);
        friend class Container3D;
    protected:
    private:
        bool            __isVisible;
    };
}
#endif
#ifndef CONTAINER_3D_H
#define CONTAINER_3D_H
#include "Controller3D.h"
#include "Global.h"
namespace MyBase3D {

class Container3D: public Controller3D {
    public:
        Container3D();
        ~Container3D();
        size_t size() const;
        void clear();
        Controller3D* operator[](const std::size_t& index);
    protected:
        virtual void    insert(Controller3D* controller),
                        erase(Controller3D* controller),
                        update()                                    override;
        virtual bool    catchEvent(GLFWwindow* window)              override;
        virtual bool    handle(GLFWwindow* window)                  override;
        virtual void    reset()                                     override;
        virtual void    glDraw()                            const   override;
        virtual void    glDrawTransparent()                 const   override;
    private:
        std::vector<Controller3D*>                __children;
        // glm::vec2       __childrenPosition, __childrenSize;
    };
};
#endif
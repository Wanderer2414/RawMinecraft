#ifndef CONTAINER_H
#define CONTAINER_H
#include "Controller.h"
#include "Global.h"
namespace MyBase {

class Container: public Controller {
    public:
        Container();
        ~Container();
        virtual bool    setHover(const glm::vec2& position)      override,
                        setHover(const bool& hover)                 override;
        Controller*     getCurrentFocus() const;
        std::size_t     size()                                      const;
        virtual void    insert(Controller* controller, const int& layer = 0),
                        erase(Controller* controller),
                        update()                                    override;
        virtual void    setFocus(const bool& focus)                 override;
        glm::vec2       getPosition() const                         override,
                        getSize() const                             override;
        virtual void    setPosition(const float& x, const float& y) override;
        virtual void    clear();
        Controller* operator[](const std::size_t& index);
    protected:
        virtual bool    sensitiveHandle(GLFWwindow* window)         override;
        virtual bool    catchEvent(GLFWwindow* window)              override;
        virtual bool    handle(GLFWwindow* window)                  override;
        virtual void    reset()                                     override;
        virtual void    glDraw()                            const   override;
        std::vector<std::pair<Controller*, int>> children;
    private:
        int             __previosFocus, __currentHover, __currentFocus;
    };
};
#endif
#ifndef CONTAINER_H
#define CONTAINER_H
#include "Controller2D.h"
#include "Global.h"
namespace MyBase {

class Container2D: public Controller2D {
    public:
        Container2D();
        ~Container2D();
        virtual bool    setHover(const glm::vec2& position)      override,
                        setHover(const bool& hover)              override;
        Controller2D*   getCurrentFocus() const;
        const int&      getCurrentFocusIndex() const;
        std::size_t     size()                                      const;
        virtual void    setFocus(const bool& focus)                 override;
        const glm::vec2& getChildrenPosition() const;
        Controller2D* operator[](const std::size_t& index);
    protected:
        void            clear();
        virtual void    insert(Controller2D* controller, const int& layer = 0),
                        erase(Controller2D* controller),
                        insertPermanent(Controller2D* controller),
                        update()                                    override;
        virtual bool    catchEvent(GLFWwindow* window)              override;
        virtual bool    handle(GLFWwindow* window)                  override;
        virtual void    reset()                                     override;
        virtual void    glDraw()                            const   override;
        virtual void    glDrawTransparent()                 const   override;
        const int&      __getCurrentFocus() const;
        const int&      __getCurrentHover() const;
        bool            __setCurrentHover(const unsigned int& index);
        void            __setCurrentFocus(const unsigned int& index);
        bool            __unsetCurrentHover();
        void            __unsetCurrentFocus();
    private:
        int             __previosFocus, __currentHover, __currentFocus;
        std::vector<Controller2D*>                __permanentChildren;
        std::vector<std::pair<Controller2D*, int>> __children;
        // glm::vec2       __childrenPosition, __childrenSize;
    };
};
#endif
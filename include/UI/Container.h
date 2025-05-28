#ifndef CONTAINER_H
#define CONTAINER_H
#include "Controller.h"
#include "Global.h"
namespace MyBase {

class Container: public Controller {
    public:
        Container();
        ~Container();
        virtual bool    setHover(const Vector2f& position)          override,
                        setHover(const bool& hover)                 override;
        std::size_t     size()                                      const;
        virtual void    insert(Controller* controller, const int& layer = 0),
                        erase(Controller* controller),
                        update()                                    override;
        virtual void    setFocus(const bool& focus)                 override;
        Vector2f        getPosition() const                         override,
                        getSize() const                             override;
        virtual void    setPosition(const float& x, const float& y) override;
        virtual void    clear();
        Controller* operator[](const std::size_t& index);
    protected:
        virtual         catch_function(CatchEvent)                  override;
        virtual         catch_function(AfterCatch)                  override;
        virtual         handle_function(handle)                     override;
        virtual void    reset()                                     override;
        virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
        vector<pair<Controller*, int>> children;
    
        int             old_focus, hovered_controller, focus_control;
    private:
    };
};
#endif
#ifndef CONTAINER_3D_H
#define CONTAINER_3D_H
#include "Controller3D.h"
#include "Global.h"
#include "Ray.h"
class Container3D: public Controller3D {
public:
    Container3D();
    ~Container3D();
    virtual bool    setHover(const Ray3f& position)          override,
                    setHover(const bool& hover)                 override;
    size_t          size()                                      const;
    virtual void    insert(Controller3D* controller, const int& layer = 0),
                    erase(Controller3D* controller),
                    update()                                    override;
    virtual void    setFocus(const bool& focus)                 override;
    virtual void    clear();
    Controller3D* operator[](const size_t& index);
protected:
    virtual         catch_function(CatchEvent)                  override;
    virtual         catch_function(AfterCatch)                  override;
    virtual         handle_function(handle)                     override;
    virtual void    reset()                                     override;
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    vector<pair<Controller3D*, int>> children;

    int             old_focus, hovered_controller, focus_control;
private:
};
#endif
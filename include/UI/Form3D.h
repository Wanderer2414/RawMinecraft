#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Container.h"
#include "Container3D.h"
#include "Global.h"

class Form3D: public Container, public Container3D {
public:
    Form3D(const int& index);
    ~Form3D();                                     
    virtual void    insert(Controller* controller, const int& layer = 0)    override,
                    erase(Controller* controller)                           override,
                    insert(Controller3D* controller, const int& layer = 0)  override,
                    erase(Controller3D* controller)                         override;
    virtual bool    contains(const Ray3f& sight) const override;
    virtual Vector2f getSize() const override;
    virtual int     run(RenderWindow& window);
protected:
    virtual         catch_function(CatchEvent) override;
    virtual         catch_function(BeforeCatch) override;
    virtual         catch_function(AfterCatch) override;
    virtual         handle_function(handle) override;
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    int             form_index, 
                    return_value;
    Camera          sCamera;
};

#endif
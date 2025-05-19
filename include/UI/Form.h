#ifndef FORM_H
#define FORM_H
#include "Container.h"
#include "Global.h"

class Form: public Container {
public:
    Form(const int& index);
    ~Form();
    virtual bool    contains(const Vector2f& position) const override;
    virtual Vector2f getSize() const override;
    virtual int     run(RenderWindow& window);
protected:
    virtual         catch_function(CatchEvent) override;
    virtual void    draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
    int             form_index, 
                    return_value;
};

#endif
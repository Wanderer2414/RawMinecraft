#ifndef FORM_H
#define FORM_H
#include "Container.h"
#include "Global.h"
namespace MyBase {

    class Form: public Container {
    public:
        Form(const int& index);
        ~Form();
        virtual bool    contains(const sf::Vector2f& position) const override;
        virtual sf::Vector2f getSize() const override;
        virtual int     run(sf::RenderWindow& window);
    protected:
        virtual         catch_function(CatchEvent) override;
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
        int             _formIndex, 
                        _returnValue;
    };
    
};
#endif
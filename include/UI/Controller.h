#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Global.h"
namespace MyBase {
    class Container;

    class Controller: public sf::Drawable {
    public:
        Controller();
        ~Controller();
        virtual bool            isHovered()         const,
                                isReleased()        const,
                                isDoubleClick()     const,
                                isPressed()         const,
                                isFocus()           const,
                                setHover(const bool& hover),
                                setHover(const sf::Vector2f& position);

        virtual sf::Vector2f    getPosition() const,
                                getSize() const;
        virtual void            setPosition(const float& x, const float& y),
                                setFocus(const bool& focus),
                                reset(),
                                update();      

        friend Container;
    protected:
        virtual         catch_function(CatchEvent);
        virtual         catch_function(BeforeCatch);
        virtual         catch_function(AfterCatch);
        virtual         handle_function(handle);
        virtual bool    contains(const sf::Vector2f& position) const;
        virtual void    draw(sf::RenderTarget& target, sf::RenderStates state) const override;

    private:
        unsigned int    __doubleClickCount;
        bool            __isHovered,
                        __isPressed,
                        __isDoubleClick,
                        __isReleased,
                        __isFocus;
    };
};
#endif
#ifndef FORM_3D_H
#define FORM_3D_H
#include "Camera.h"
#include "Container.h"
#include "Container3D.h"
#include "Global.h"
namespace MyBase3D {
    class Form3D: public MyBase::Container, public Container3D {
        public:
            Form3D(const int& index);
            ~Form3D();                                     
            virtual void            insert(Controller* controller, const int& layer = 0)    override,
                                    erase(Controller* controller)                           override,
                                    insert(Controller3D* controller, const int& layer = 0)  override,
                                    erase(Controller3D* controller)                         override;
            virtual bool            contains(const Ray3f& sight) const override;
            virtual sf::Vector2f    getSize() const override;
            virtual int             run(sf::RenderWindow& window);
        protected:
            virtual         catch_function(CatchEvent) override;
            virtual         catch_function(BeforeCatch) override;
            virtual         catch_function(AfterCatch) override;
            virtual         handle_function(handle) override;
            virtual void    draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;
            int             _formIndex, 
                            _returnValue;
            Camera          _camera;
        };       
}
#endif
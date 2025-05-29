#ifndef APPLICATION_H
#define APPLICATION_H
#include "Global.h"
namespace MyBase {
    class Application {
        public:
            Application(const sf::Vector2f& window_size, const std::string& title);
            ~Application();
            
            virtual void run();
        private:
            sf::RenderWindow __window;
    };        
}
#endif
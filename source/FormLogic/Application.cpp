#include "Application.h"
#include "GameForm.h"
#include "SFML/Window/WindowStyle.hpp"
#include <GL/gl.h>

Vector2f WindowSize;

Application::Application(const Vector2f& window_size, const string& title) {
    ContextSettings settings;
    settings.depthBits = 24; 
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    WindowSize = window_size ;

    window.create(VideoMode(window_size.x, window_size.y), title, Style::Default, settings);
    Mouse::setPosition(static_cast<Vector2i>(window.getPosition() + static_cast<Vector2i>(WindowSize)/2), window);
    window.display();
}
void Application::run() {
    int form_index = 0;
    while (window.isOpen()) {
        switch (form_index) {
            case 0: {
                GameForm gameForm(window, 0);
                form_index = gameForm.run(window);
            };
            break;
        }
    }
}
Application::~Application() {

}
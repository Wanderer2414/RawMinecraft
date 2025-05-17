#include "Application.h"
#include "GameForm.h"
#include <GL/gl.h>

Vector2f WindowSize;

Application::Application(const Vector2f& window_size, const string& title) {
    WindowSize = window_size ;
    window.create(VideoMode(window_size.x, window_size.y), title);
    window.display();

}
void Application::run() {
    int form_index = 0;
    while (window.isOpen()) {
        switch (form_index) {
            case 0: {
                GameForm gameForm(0);
                form_index = gameForm.run(window);
            };
            break;
        }
    }
}
Application::~Application() {

}
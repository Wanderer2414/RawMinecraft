#include "Application.h"
#include "GameForm.h"

Vector2f WindowSize;

Application::Application(const Vector2f& window_size, const string& title) {
    WindowSize = window_size ;
    window.create(VideoMode(window_size.x, window_size.y), title);
    window.display();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, window_size.x/window_size.y, 1, 40);
  
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 6, 5, 0, 0, 0, 0, 1, 0);

    glViewport(0, 0, window_size.x, window_size.y);
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
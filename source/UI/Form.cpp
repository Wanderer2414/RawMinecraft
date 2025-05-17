#include "Form.h"
#include "Container.h"
#include "Controller.h"
#include "Global.h"
#include <GL/gl.h>

extern Vector2f WindowSize;

Form::Form(const int& index) {
    form_index = index;
    return_value = INT_MIN;
}
Form::~Form() {

}
Vector2f Form::getSize() const {
    return WindowSize;
}
_catch_function(Form, CatchEvent) {
    bool ans = false;
    if (event.type == Event::Resized) {
        FloatRect rect = {0, 0, 1.0f*event.size.width, 1.0f*event.size.height};
        window.setView(View(rect));
        ans = true;
    }
    ans = Container::CatchEvent(window, event, state) || ans;
    return ans;
}
bool Form::contains(const Vector2f& position) const {
    return true;
}
int Form::run(RenderWindow& window) {
    Event event;
    bool is_changed = true, is_catched = false;
    while (window.isOpen()) {
        reset();
        while (window.pollEvent(event)) {
            if (!is_catched) {
                is_catched = true;
                is_changed = BeforeCatch(window, event) || is_changed;
                is_changed = setHover(static_cast<Vector2f>(Mouse::getPosition(window))) || is_changed;
            }
            is_changed = CatchEvent(window, event) || is_changed;
            if (event.type == Event::Closed) window.close();
        }
        if (is_catched) {
            is_changed = AfterCatch(window, event) || is_changed;
            is_catched = false;
        }
        is_changed = handle(window) || is_changed;
        if (is_changed) {
            window.clear();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0, 0, 0, 0);            
            draw(window);
            window.popGLStates();
            glBegin(GL_LINES);
                glColor3f(1, 0, 0);glVertex3f(0, 0,0); glVertex3f(10, 0, 0);
                glColor3f(0, 1, 0);glVertex3f(0, 0,0); glVertex3f(0, 10, 0);
                glColor3f(0, 0, 1);glVertex3f(0, 0,0); glVertex3f(0, 0, 10);
            glEnd();
            window.pushGLStates();
            glFlush();
            window.display();
        }
        if (return_value!=INT_MIN) return return_value;
        is_changed = 0;
    }
    return form_index;
}
void Form::draw(RenderTarget& target, RenderStates state) const {
    for (const auto& [child, layer]:children) 
        if (focus_control == -1 || child != children[focus_control].first) target.draw(*child, state);
    if (focus_control != -1) target.draw(*children[focus_control].first, state);
}
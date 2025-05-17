#include "GameForm.h"
#include "Global.h"
#include "SFML/Window/Keyboard.hpp"

GameForm::GameForm(const int& index):Form(index) {
    
    insert(&cubes);
    
    cubes.setOutlineThickness(1);
    cubes.setSize(1);
    cubes.Cube::setPosition(0, 0, 0);

    camera.setPosition(4, 4, 2);
    camera.setCenter(0, 0, 2);
    camera.setFarProjection(40);
    camera.setNearProjection(1);
    camera.setWide(60);
}
GameForm::~GameForm() {

}
bool GameForm::setHover(const Vector2f& position) {
    bool is_changed = Form::setHover(position);
    return is_changed;
}
_catch_function(GameForm, CatchEvent) {
    bool is_changed = Form::CatchEvent(window, event, state);
    return is_changed;
}
_catch_function(GameForm, AfterCatch) {
    bool is_changed = Form::AfterCatch(window, event, state);
    return is_changed;
}
_handle_function(GameForm, handle) {
    bool is_changed = Form::handle(window);
    if (Keyboard::isKeyPressed(Keyboard::Scancode::A)) {
        window.popGLStates();
            camera.move(0.01, 0, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::D)) {
        window.popGLStates();
            camera.move(-0.01, 0, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::W)) {
        window.popGLStates();
            camera.move(0, 0.01, 0);
        window.pushGLStates();
        is_changed = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::Scancode::S)) {
        window.popGLStates();
            camera.move(0, -0.01, 0);
        window.pushGLStates();
        is_changed = true;
    }
    return is_changed;
}
void GameForm::draw(RenderTarget& target, RenderStates state) const {
    Form::draw(target, state);
    target.popGLStates();
    glBegin(GL_LINES);
        glVertex3f(0, 0,0);
        auto vector = camera.getHorizontalVector();
        glVertex3f(vector.x, vector.y, vector.z);
    glEnd();
    target.pushGLStates();
}
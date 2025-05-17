#include "GameForm.h"
#include "SFML/Window/Keyboard.hpp"

GameForm::GameForm(const int& index):Form(index) {
    
    insert(&cubes);
    
    cubes.setOutlineThickness(1);
    cubes.setSize(1);
    cubes.Cube::setPosition(0, 0, 0);

    camera.setPosition(2, 2, 2);
    camera.setCenter(0, 0, 0);
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

_catch_function(GameForm, AfterCatch) {
    bool is_changed = Form::AfterCatch(window, event, state);
    if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Scancode::Left)) {
        
    }
    return is_changed;
}
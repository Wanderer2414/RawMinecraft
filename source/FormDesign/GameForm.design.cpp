#include "GameForm.h"
#include "Global.h"

extern Vector2f WindowSize;

GameForm::GameForm(RenderWindow& window, const int& index):Form(index) {
    
    insert(&cubes);
    insert(&camera);
    
    cubes.setOutlineThickness(1);
    cubes.setSize(1);
    cubes.Cube::setPosition(0, 0, 0);

    camera.setPosition(4, 4, 2);
    camera.setCenter(1, 1, 2);
    camera.setFarProjection(40);
    camera.setNearProjection(1);
    camera.setWide(60);

    window.setMouseCursorVisible(false);
    Mouse::setPosition(static_cast<Vector2i>(WindowSize)/2, window);

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
    return is_changed;
}
void GameForm::draw(RenderTarget& target, RenderStates state) const {
    Form::draw(target, state);
}
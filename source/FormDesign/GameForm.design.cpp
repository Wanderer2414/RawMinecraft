#include "GameForm.h"
#include "Global.h"

extern Vector2f WindowSize;

GameForm::GameForm(RenderWindow& window, const int& index): Form3D(index) {
    
    insert(&coordinate);
    insert(&cubes);
    insert(&track_rect);
    
    cubes.setOutlineThickness(1);
    cubes.setSize(1);
    cubes.Cube::setPosition(0, 0, 0);

    track_rect.setPosition(Vector3f(2, 0, 0), Vector3f(2, 0, 3), Vector3f(2, 4, 3));

    sCamera.setPosition(4, 4, 2);
    sCamera.setCenter(1, 1, 2);
    sCamera.setFarProjection(40);
    sCamera.setNearProjection(1);
    sCamera.setWide(60);

    window.setMouseCursorVisible(false);
    Mouse::setPosition(static_cast<Vector2i>(WindowSize)/2, window);

}
GameForm::~GameForm() {

}
_catch_function(GameForm, CatchEvent) {
    bool is_changed = Form3D::CatchEvent(window, event, state);
    return is_changed;
}
_catch_function(GameForm, AfterCatch) {
    bool is_changed = Form3D::AfterCatch(window, event, state);
    return is_changed;
}
_handle_function(GameForm, handle) {
    bool is_changed = Form3D::handle(window);
    return is_changed;
}
void GameForm::draw(RenderTarget& target, RenderStates state) const {
    Form3D::draw(target, state);
}
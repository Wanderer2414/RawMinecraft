#include "GameForm.h"

GameForm::GameForm(const int& index):Form(index) {

}
GameForm::~GameForm() {

}
bool GameForm::setHover(const Vector2f& position) {
    bool is_chanaged = Form::setHover(position);
    return is_chanaged;
}
float x = 1;
_catch_function(GameForm, AfterCatch) {
    bool is_changed = Form::AfterCatch(window, event, state);
    return true;
}
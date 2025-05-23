#include "GameForm.h"
#include "Block.h"
#include "Global.h"
#include <GL/gl.h>

extern Vector2f WindowSize;

GameForm::GameForm(RenderWindow& window, const int& index): Form3D(index) {
    insert(&pCoordinate);
    // insert(&cubes);
    // cubes.type = BlockCatogary::Dirt;
    // for (int i = 0; i<3; i++) {
    //     for (int j = 0; j<3; j++) {
    //         for (int k = 0; k<3; k++) {
    //             insert(&chunks[i][j][k]);
    //             chunks[i][j][k].setPosition((i-1)*16,(j-1)*16,(k-1)*16);
    //         }
    //     }
    // }
    // cubes.setPosition(0, 0, 0);

    sCamera.setPosition(4, 4, 2);
    sCamera.setCenter(1, 1, 2);
    sCamera.setFarProjection(100);
    sCamera.setNearProjection(0.1);
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
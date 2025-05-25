#include "GameForm.h"
#include "Block.h"
#include "Global.h"
#include <GL/gl.h>

extern Vector2f WindowSize;

GameForm::GameForm(RenderWindow& window, const int& index): Form3D(index), pWorld(0, 0, 0) {
    insert(&pWorld);
    for (int i = 0; i<16; i++) {
        for (int j = 0; j<16; j++) {
            pWorld.at(i, j, -1) = BlockCatogary::Dirt;
        }
    }
    pWorld.at(5, 5, 0) = BlockCatogary::Dirt;
    pWorld.at(0, 0, 0) = BlockCatogary::Dirt;
    pWorld.at(0, 0, 1) = BlockCatogary::Dirt;
    pWorld.at(0, 0, 2) = BlockCatogary::Dirt;

    pWorld.at(-2, 0, 0) = BlockCatogary::Dirt;
    pWorld.at(-2, 0, 1) = BlockCatogary::Dirt;
    pWorld.at(-2, 0, 2) = BlockCatogary::Dirt;

    window.setMouseCursorVisible(false);
    Mouse::setPosition(static_cast<Vector2i>(WindowSize)/2, window);

    sCamera.setPosition({2, 2, 1.7});
    pZVelocity = 0;
    pWindowCenter = static_cast<Vector2i>(WindowSize/2.f);
    pSpeed = 0.1;
    pFrameAlarm.setDuration(1.f/60);
}
GameForm::~GameForm() {

}
bool GameForm::move(const float& x, const float& y, const float& z) {
    glm::vec3 delta = {0, 0, 0}, pos= sCamera.getPosition();
    delta += x*sCamera.getHorizontalVector();
    glm::vec3 tmp = sCamera.getDirection();
    tmp.z = 0;
    tmp /= glm::length(tmp);
    delta.x += y*tmp.x;
    delta.y += y*tmp.y;
    delta.z += z;
    float cX = pos.x+delta.x;
    float cY = pos.y+delta.y;
    if (delta.x>0) cX += 0.2;
    if (delta.x<0) cX -= 0.2;
    if (delta.y>0) cY += 0.2;
    if (delta.y<0) cY -= 0.2;
    bool above = pWorld.at(floor(cX), floor(cY), floor(pos.z+delta.z)) == BlockCatogary::Air;
    bool below = pWorld.at(floor(cX), floor(cY), floor(pos.z+delta.z-1.7)) == BlockCatogary::Air;
    if (above && below) {
        sCamera.setPosition(delta+pos);
        return true;
    }
    else if (above && !below) {
        if (!pZVelocity && pWorld.at(floor(cX), floor(cY), floor(pos.z+delta.z+1)) == BlockCatogary::Air) {
            sCamera.setPosition(delta+pos+glm::vec3(0, 0, 1));
            return true;
        }
    }
    return false;
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

    if (pFrameAlarm.get()) {
        if (Keyboard::isKeyPressed(Keyboard::Scancode::LControl)) {
            pSpeed = 0.2;
        }
        if (pZVelocity>-40) {
            if (Keyboard::isKeyPressed(Keyboard::Scancode::A)) {
                move(pSpeed, 0, 0);
                is_changed = true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Scancode::D)) {
                move(-pSpeed, 0, 0);
                is_changed = true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Scancode::W)) {
                move(0, pSpeed, 0);
                is_changed = true;
            } else pSpeed = 0.1;
            if (Keyboard::isKeyPressed(Keyboard::Scancode::S)) {
                move(0, -pSpeed, 0);
                is_changed = true;
            }
        }

        if (pZVelocity>-50 && pWorld.at(floor(sCamera.getPosition().x-0.3), floor(sCamera.getPosition().y-0.3), floor(sCamera.getPosition().z-2)) == BlockCatogary::Air &&
            pWorld.at(floor(sCamera.getPosition().x-0.3), floor(sCamera.getPosition().y+0.3), floor(sCamera.getPosition().z-2)) == BlockCatogary::Air &&
            pWorld.at(floor(sCamera.getPosition().x+0.3), floor(sCamera.getPosition().y+0.3), floor(sCamera.getPosition().z-2)) == BlockCatogary::Air &&
            pWorld.at(floor(sCamera.getPosition().x+0.3), floor(sCamera.getPosition().y-0.3), floor(sCamera.getPosition().z-2)) == BlockCatogary::Air) {
            pZVelocity -= 3;
        }
        else if (!pZVelocity && Keyboard::isKeyPressed(Keyboard::Key::Space)) {
            pZVelocity = 30;
        }
        if (pZVelocity>0) {
            move(0, 0, pZVelocity/150);
            is_changed = true;
        }
        else if (pZVelocity<0) {
            if (!move(0, 0, pZVelocity/200)) {
                pZVelocity = 0;
            };
            is_changed = true;
        }
    }

    Vector2i position = Mouse::getPosition(window);
    Vector2f delta;
    delta.x = position.x - pWindowCenter.x;
    delta.y = position.y - pWindowCenter.y;

    if (delta.x != 0 || delta.y != 0) {
        Mouse::setPosition(pWindowCenter, window);
        sCamera.rotate(delta.y/1000, delta.x/1000);
        is_changed = true;
    }
    return is_changed;
}
void GameForm::draw(RenderTarget& target, RenderStates state) const {
    Form3D::draw(target, state);
}
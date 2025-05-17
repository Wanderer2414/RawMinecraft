#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Alarm.h"
#include "Camera.h"
#include "Cube.h"
#include "Form.h"
#include "Global.h"
#include "ShapeShow.h"
class GameForm: public Form {
public:
    GameForm(RenderWindow& window, const int& index);
    ~GameForm();
    virtual bool setHover(const Vector2f& position) override;
protected:
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    catch_function(CatchEvent) override;
    catch_function(AfterCatch) override;
    handle_function(handle)     override;

    ShapeShow<Cube> cubes;

    Camera          camera;
};
#endif
#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Alarm.h"
#include "Camera.h"
#include "Cube.h"
#include "Form3D.h"
#include "Global.h"
#include "ShapeShow.h"
class GameForm: public Form3D {
public:
    GameForm(RenderWindow& window, const int& index);
    ~GameForm();
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
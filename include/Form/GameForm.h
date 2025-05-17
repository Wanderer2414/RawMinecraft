#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Camera.h"
#include "Cube.h"
#include "Form.h"
#include "ShapeShow.h"
class GameForm: public Form {
public:
    GameForm(const int& index);
    ~GameForm();
    virtual bool setHover(const Vector2f& position) override;
private:
    catch_function(AfterCatch) override;

    ShapeShow<Cube> cubes;

    Camera          camera;
};
#endif
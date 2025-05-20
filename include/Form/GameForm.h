#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Camera.h"
#include "Coordinate.h"
#include "Cube.h"
#include "Form3D.h"
#include "Global.h"
#include "TrackRect.h"
class GameForm: public Form3D {
public:
    GameForm(RenderWindow& window, const int& index);
    ~GameForm();
protected:
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    catch_function(CatchEvent) override;
    catch_function(AfterCatch) override;
    handle_function(handle)    override;

    Cube            cubes;

    Coordinate      coordinate; 
};
#endif
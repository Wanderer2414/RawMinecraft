#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Alarm.h"
#include "Camera.h"
#include "Chunk.h"
#include "Block.h"
#include "Coordinate.h"
#include "Form3D.h"
#include "Global.h"
#include "World.h"

class GameForm: public Form3D {
public:
    GameForm(RenderWindow& window, const int& index);
    ~GameForm();

    bool move(const float& x, const float& y, const float& z);
protected:
    virtual void draw(RenderTarget& target, RenderStates state = RenderStates::Default) const override;
private:
    float   pZVelocity;
    catch_function(CatchEvent) override;
    catch_function(AfterCatch) override;
    handle_function(handle)    override;
    Vector2i pWindowCenter;
    Alarm   pFrameAlarm;
    World   pWorld;
    float   pSpeed;
};
#endif
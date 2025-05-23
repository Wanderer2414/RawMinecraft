#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Camera.h"
#include "Chunk.h"
#include "Block.h"
#include "Coordinate.h"
#include "Form3D.h"
#include "Global.h"

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

    // Block            cubes;
    // BlockCatogary   pBlockCatogary;
    Coordinate      pCoordinate;
    // Chunk           chunks[3][3][3];
};
#endif
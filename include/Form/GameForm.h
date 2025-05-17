#ifndef GAMEFORM_H
#define GAMEFORM_H
#include "Form.h"
class GameForm: public Form {
public:
    GameForm(const int& index);
    ~GameForm();
    virtual bool setHover(const Vector2f& position) override;
private:
    catch_function(AfterCatch) override;
};
#endif
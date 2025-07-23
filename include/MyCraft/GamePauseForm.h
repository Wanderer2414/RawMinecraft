#ifndef GAME_PAUSE_H
#define GAME_PAUSE_H
#include "MessageBox.h"
namespace MyCraft {
    class GamePauseForm: public MyBase::MessageBox {
    public:
        GamePauseForm();
        GamePauseForm(const GamePauseForm&) = delete;
        ~GamePauseForm();
        GamePauseForm& operator=(const GamePauseForm&) const = delete; 
    protected:
    private:
        
    };
}
#endif
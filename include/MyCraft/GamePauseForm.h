#ifndef GAME_PAUSE_H
#define GAME_PAUSE_H
#include "Button.h"
#include "Font.h"
#include "MessageBox.h"
namespace MyCraft {
    class GamePauseForm: public MyBase::MessageBox {
    public:
        GamePauseForm(const MyBase::Font& font);
        GamePauseForm(const GamePauseForm&) = delete;
        ~GamePauseForm();
        GamePauseForm& operator=(const GamePauseForm&) const = delete; 
    protected:
        bool __mouseClicked(GLFWwindow*) override;
    private:
        MyBase::RoundedRectangleButton  __exitButton;
        
    };
}
#endif
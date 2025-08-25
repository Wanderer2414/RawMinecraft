#ifndef GAME_PAUSE_H
#define GAME_PAUSE_H
#include "Button.h"
#include "Font.h"
#include "Label.h"
#include "MessageBox.h"
#include "Slider.h"
namespace MyCraft {
    class GamePauseForm: public MyBase::MessageBox {
    public:
        GamePauseForm(const MyBase::Font& font);
        GamePauseForm(const GamePauseForm&) = delete;
        ~GamePauseForm();
        GamePauseForm& operator=(const GamePauseForm&) const = delete; 
    protected:
        bool __mouseClicked(GLFWwindow*) override;
        void __open(GLFWwindow*) override;
        void __close(GLFWwindow*) override;
    private:
        bool handle(GLFWwindow*) override;
        MyBase::RoundedRectangleButton  __exitButton;
        MyBase::Slider          __volumeBar; 
        MyBase::Label           __volumnLabel;
         
    };
}
#endif
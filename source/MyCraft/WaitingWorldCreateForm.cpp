#include "WaitingWorldCreateForm.h"
#include "Font.h"
#include "General.h"
#include "MapCreator.h"
#include "ProgressWaitingForm.h"
#include "Global.h"
#include <thread>
namespace MyCraft {
    WaitingWorldCreateForm::WaitingWorldCreateForm(const MyBase::Font& font): MyBase::ProgressWaitingForm(font)  {}
    WaitingWorldCreateForm::~WaitingWorldCreateForm() {}
    int WaitingWorldCreateForm::open(const std::string& file, GLFWwindow* window) {
        thread = new std::thread(MapCreator::create, &progress, file);
        time = GetTime();
        return MyBase::ProgressWaitingForm::open(window);
    }
    void WaitingWorldCreateForm::__close(GLFWwindow*) {
        std::cout << GetTime()-time << "s" << std::endl;
        thread->join();
        delete thread;
    }
    bool WaitingWorldCreateForm::__exitCondition() {
        return std::round(progress*100) == 100;
    }
}
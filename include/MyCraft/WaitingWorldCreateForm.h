#ifndef WAITING_WORLD_CREATE_FORM
#define WAITING_WORLD_CREATE_FORM
#include "Font.h"
#include "ProgressWaitingForm.h"
#include <thread>
namespace MyCraft {
    class WaitingWorldCreateForm: public MyBase::ProgressWaitingForm {
    public:
        WaitingWorldCreateForm(const MyBase::Font& font);
        WaitingWorldCreateForm(const WaitingWorldCreateForm&) = delete;
        ~WaitingWorldCreateForm();
        WaitingWorldCreateForm& operator=(const WaitingWorldCreateForm&) const = delete; 
        int open(const std::string& file, GLFWwindow* window);
    protected:
        void __close(GLFWwindow*) override;
        bool __exitCondition() override;
    private:
        std::thread* thread;
    };
}
#endif
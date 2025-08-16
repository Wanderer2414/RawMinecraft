#ifndef PROGRESS_WAITING_FORM_H
#define PROGRESS_WAITING_FORM_H
#include "Label.h"
#include "MessageBox.h"
namespace MyBase {
    class ProgressWaitingForm: public MessageBox {
    public:
        ProgressWaitingForm(const Font& font);
        ProgressWaitingForm(const ProgressWaitingForm&) = delete;
        ~ProgressWaitingForm();
        ProgressWaitingForm& operator=(const ProgressWaitingForm&) const = delete; 
    protected:
        double progress;
        std::mutex mtx;
        virtual bool catchEvent(GLFWwindow* window) override;
    private:
        int     old_percent;
        Label   __waitingLabel;
    };
}
#endif
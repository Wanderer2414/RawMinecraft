#include "ProgressWaitingForm.h"
#include "Color.h"
#include "MessageBox.h"
#include <cmath>
#include <limits>
#include <mutex>
#include <thread>
#include <string.h>
namespace MyBase {
    ProgressWaitingForm::ProgressWaitingForm(const Font& font): progress(0), old_percent(0) {
        setSize({0.6, 0.3}, 0.01);
        setPosition({-0.3, -0.15});
        setFillColor(WHITE);

        __waitingLabel.setFont(font);
        __waitingLabel.setTextColor(BLACK);
        __waitingLabel.setScale({0.1, 0.15});
        __waitingLabel.setText("0%");
        __waitingLabel.setPosition({-__waitingLabel.getSize().x/2, -0.1});
        insert(&__waitingLabel);
    }
    ProgressWaitingForm::~ProgressWaitingForm() {}
    bool ProgressWaitingForm::catchEvent(GLFWwindow* window) {
        bool is_changed = MessageBox::catchEvent(window);
        std::lock_guard<std::mutex> lock(mtx);
        int percent = std::round(progress*10000);
        if (percent != old_percent) {
            old_percent = percent;
            __waitingLabel.setText(std::format("{}.{}%",old_percent/100, old_percent%100));
            __waitingLabel.setPosition({-__waitingLabel.getSize().x/2, -0.1});
            is_changed = true;
        }
        return is_changed;
    }
}
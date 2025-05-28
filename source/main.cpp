#include "Application.h"

int main() {
    MyBase::Application application({1366, 748}, "Data center");
    application.run();
    return 0;
}
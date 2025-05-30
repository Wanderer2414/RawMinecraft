#include "Application.h"

int main() {
    MyBase::Application application({1366, 748}, "MyCraft");
    application.run();
    return 0;
}
#ifndef APPLICATION_H
#define APPLICATION_H
#include "Global.h"

class Application {
public:
    Application(const Vector2f& window_size, const string& title);
    virtual void run();
    ~Application();
private:
    RenderWindow window;
};

#endif
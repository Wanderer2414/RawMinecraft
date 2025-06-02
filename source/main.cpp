#include "Global.h"
#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    MyCraft::Application application(800, 600);
    application.run();
    return 0;
}
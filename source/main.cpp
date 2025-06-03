#include "Global.h"
#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main() {
    MyCraft::Application application(1366, 768);
    application.run();
    return 0;
}
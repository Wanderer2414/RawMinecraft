#include "Global.h"
#include "Application.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

int main() {
    MyCraft::Application application(1366, 768);
    application.run();
    return 0;
}
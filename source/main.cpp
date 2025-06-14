
// #define GLM_ENABLE_EXPERIMENTAL
// #include "glm/gtx/quaternion.hpp"

#include "Global.h"
#include "Application.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    MyCraft::Application application(1366, 768);
    application.run();
    return 0;
}
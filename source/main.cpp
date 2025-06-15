#include "General.h"
#include "Global.h"
#include "Application.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    auto sz = MyBase::getWindowSize();
    MyCraft::Application application(sz.x, sz.y);
    application.run();
    return 0;
}
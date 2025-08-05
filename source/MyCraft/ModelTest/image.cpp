#include "image.h"

image util::load_from_file(const std::string& filename) {
    image img;
    img.pixels = stbi_load(filename.c_str(), &img.width, &img.height, &img.channels, STBI_rgb_alpha);
    if (!img.pixels) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        img.width = img.height = img.channels = 0;
    }
    return img;
}

void util::free_image_memory(image img) {
    if (img.pixels) {
        stbi_image_free(img.pixels);
        img.pixels = nullptr;
    }
    img.width = img.height = img.channels = 0;
}
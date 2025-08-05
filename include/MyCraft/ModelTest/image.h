#pragma once
#include "config.h"
#include "stb_image.h"

struct image{
    unsigned char * pixels;
    int width, height, channels;    
};

namespace util{
    image load_from_file(const std::string& filename);
    void free_image_memory(image img);
};
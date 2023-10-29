#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned char *stbi_load(std::string file, int *width, int *height, int *n_channel, int desired_channels)
{
    unsigned char *buffer = stbi_load(file.c_str(), width, height, n_channel, desired_channels);
    if(!buffer) {
        std::cout << "ERROR::LOAD::IMAGE <" << file << ">" << std::endl;
        return nullptr;
    }
    std::cout << "LOAD::IMAGE <" << file << "> WIDTH <" << *width << ">" << " HEIGHT <" << *height << ">" << std::endl;
    return buffer;
}

void stbi_image_free(unsigned char *buffer)
{
    STBI_FREE(buffer);
}



#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <string>

unsigned char *stbi_load(std::string file, int *width, int *height, int *n_channel, int desired_channels = 0);
void stbi_image_free(unsigned char *buffer);

#endif // IMAGE_HPP

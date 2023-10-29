#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <iostream>
#include <string>

unsigned char *_stbi_load(std::string file, int *width, int *height, int *n_channel, int desired_channels = 0);
void _stbi_set_flip_vertically_on_load(bool);
void _stbi_image_free(unsigned char *buffer);

#endif // IMAGE_HPP

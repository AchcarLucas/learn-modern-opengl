#include "texture.hpp"

Texture2D::Texture2D(const std::string file, const TextureType type)
{
    int width, height, n_channel;
    unsigned char *image = _stbi_load(file.c_str(), &width, &height, &n_channel);

    if(!image) {
        _stbi_image_free(image);
        std::cout << "ERROR::LOAD::TEXTURE_2D" << std::endl;
        return;
    }

    this->width = width;
    this->height = height;

    switch(n_channel) {
        case 1:
            this->format = GL_RED;
            break;
        case 3:
            this->format = GL_RGB;
            break;
        case 4:
            this->format = GL_RGBA;
            break;
    }

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, this->format, width, height, 0, this->format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::cout << "LOAD::TEXTURE_2D <" << file << ">:<" << textureTypeMap[type] << ">" << std::endl;

    this->type = type;
    this->file = file;

    _stbi_image_free(image);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &this->texture);
}

void Texture2D::bind(GLenum _GL_TEXTURE)
{
    glActiveTexture(_GL_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

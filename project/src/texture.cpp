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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, this->format, width, height, 0, this->format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "LOAD::TEXTURE_2D <" << file << ">:<" << textureTypeMap[type] << ">" << std::endl;

    this->type = type;
    this->file = file;

    _stbi_image_free(image);
}

Texture2D::Texture2D(const int width, const int height, const TextureType type)
{
    this->width = width;
    this->height = height;
    this->type = type;

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
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

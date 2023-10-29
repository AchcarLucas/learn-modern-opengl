#include "texture.hpp"

Texture2D::Texture2D(std::string file, bool alpha_channel)
{
    glGenTextures(1, &this->texture);

    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, n_channel;
    this->image = stbi_load(file.c_str(), &width, &height, &n_channel);

    if(!this->image) {
        std::cout << "ERROR:LOAD::TEXTURE_2D" << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, alpha_channel ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, this->image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(this->image);
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

#include "texture.hpp"

static GLenum ImageTypeFormat(int format)
{
    switch(format) {
        case 1:
            return GL_RED;
            break;
        case 3:
            return GL_RGB;
            break;
        case 4:
            return GL_RGBA;
            break;
    }

    return GL_NONE;
}

Texture2D::Texture2D(const std::string file, const TextureType type, bool flip)
{
    _stbi_set_flip_vertically_on_load(flip);

    int width, height, n_channel;
    unsigned char *image = _stbi_load(file.c_str(), &width, &height, &n_channel);

    if(!image) {
        _stbi_image_free(image);
        std::cout << "ERROR::LOAD::TEXTURE_2D" << std::endl;
        return;
    }

    this->width = width;
    this->height = height;
    this->format = ImageTypeFormat(n_channel);

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

TextureCube::TextureCube(std::vector<std::string> files, bool flip)
{
    _stbi_set_flip_vertically_on_load(flip);

    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    int width, height, n_channel;

    for (unsigned int i = 0; i < files.size(); ++i) {
        unsigned char *data = _stbi_load(files[i].c_str(), &width, &height, &n_channel, 0);

        if (!data) {
            std::cout << "ERROR::LOAD::CUBEMAP: " << files[i] << std::endl;
            _stbi_image_free(data);
            delete this;
            return;
        }

        this->format = ImageTypeFormat(n_channel);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, this->format, width, height, 0, this->format, GL_UNSIGNED_BYTE, data);
        _stbi_image_free(data);

        std::cout << "LOAD::TEXTURE_CUBEMAP <" << files[i] << ">" << std::endl;
    }

    this->files = files;
    this->width = width;
    this->height = height;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

TextureCube::~TextureCube()
{
    glDeleteTextures(1, &this->texture);
}

void TextureCube::bind(GLenum _GL_TEXTURE)
{
    glActiveTexture(_GL_TEXTURE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
}

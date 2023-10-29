#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
    public:
        Shader(std::string, std::string);
        virtual ~Shader();

        void use();

        GLuint getShaderID();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

    protected:
        GLuint _id;

        void getShaderLog(GLuint, std::string, std::string);
        void getProgramLog(GLuint, std::string, std::string);

    private:
};

#endif // SHADER_HPP

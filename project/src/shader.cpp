#include "shader.hpp"

Shader::Shader(std::string vs_path, std::string fs_path)
{
    std::string vs_code;
    std::string fs_code;

    std::ifstream vs_file;
    std::ifstream fs_file;

    // garante que o objeto do ifstream podem lançar exceções
    vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "VERTEX SHADER FILE [" << vs_path << "]" << std::endl;
    std::cout << "FRAGMENT SHADER FILE [" << fs_path << "]" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    try {
        // faz a abertura dos shaderes
        vs_file.open(vs_path);
        fs_file.open(fs_path);

        std::stringstream vs_stream, fs_stream;

        // leia os arquivos e coloque nos bufferes
        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        // não precisamos mais dos arquivos abertos, podemos fecha-los
        vs_file.close();
        fs_file.close();

        // vamos colocar
        vs_code = vs_stream.str();
        fs_code = fs_stream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    unsigned int vertex, fragment;

    const char *const_char_code;

    // compila o vertex shader
    const_char_code = vs_code.c_str();
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &const_char_code, NULL);
    glCompileShader(vertex);
    getShaderLog(vertex, "SHADER::VERTEX::COMPILATION_SUCCESSFULLY", "ERROR::SHADER::VERTEX::COMPILATION_FAILED");

    // compila o fragment shader
    const_char_code = fs_code.c_str();
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &const_char_code, NULL);
    glCompileShader(fragment);
    getShaderLog(fragment, "SHADER::FRAGMENT::COMPILATION_SUCCESSFULLY", "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");

    // faz o link do vertex shader com o fragment shader
    this->_id = glCreateProgram();
    glAttachShader(this->_id, vertex);
    glAttachShader(this->_id, fragment);
    glLinkProgram(this->_id);
    getProgramLog(this->_id, "SHADER::PROGRAM::LINKING_SUCCESSFULLY", "ERROR::SHADER::PROGRAM::LINKING_FAILED");

    // não precisamos mais do vertex e do fragment shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "----------------------------------------" << std::endl;
}

Shader::~Shader()
{
    glDeleteProgram(this->_id);
}

void Shader::use()
{
    glUseProgram(this->_id);
}

GLuint Shader::getShaderID()
{
    return this->_id;
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(this->_id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(this->_id, name.c_str()), value);
}

void Shader::getShaderLog(GLuint shader, std::string success_message, std::string error_message)
{
    char log[512];
    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(success) {
        std::cout << success_message << std::endl;
        return;
    }

    glGetShaderInfoLog(shader, 512, NULL, log);
    std::cout << error_message << log << std::endl;
}

void Shader::getProgramLog(GLuint program, std::string success_message, std::string error_message)
{
    char log[512];
    int success;

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if(success) {
        std::cout << success_message << std::endl;
        return;
    }

    glGetProgramInfoLog(program, 512, NULL, log);
    std::cout << error_message << log << std::endl;
}

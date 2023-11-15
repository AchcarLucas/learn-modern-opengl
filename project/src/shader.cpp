#include "shader.hpp"

Shader::Shader(const std::string &vs_path, const std::string &fs_path, const std::string &gs_path)
{
    std::string vs_code;
    std::string fs_code;
    std::string gs_code;

    std::ifstream vs_file;
    std::ifstream fs_file;
    std::ifstream gs_file;

    // garante que o objeto do ifstream podem lançar exceções
    vs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fs_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "VERTEX SHADER FILE [" << vs_path << "]" << std::endl;
    std::cout << "FRAGMENT SHADER FILE [" << fs_path << "]" << std::endl;
    if(!gs_path.empty()) {
        std::cout << "GEOMETRY SHADER FILE [" << gs_path << "]" << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    try {
        std::stringstream vs_stream, fs_stream, gs_stream;

        // vertex shader
        vs_file.open(vs_path);
        vs_stream << vs_file.rdbuf();
        vs_file.close();
        vs_code = vs_stream.str();

        // fragment shader
        fs_file.open(fs_path);
        fs_stream << fs_file.rdbuf();
        fs_file.close();
        fs_code = fs_stream.str();

        // geometry shader é opcional
        if(!gs_path.empty()) {
            gs_file.open(gs_path);
            gs_stream << gs_file.rdbuf();
            gs_file.close();
            gs_code = gs_stream.str();
        }
    }
    catch(std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return;
    }

    unsigned int vertex, fragment, geometry;

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

    if(!gs_path.empty()) {
        const_char_code = gs_code.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &const_char_code, NULL);
        glCompileShader(geometry);
        getShaderLog(geometry, "SHADER::GEOMETRY::COMPILATION_SUCCESSFULLY", "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED");
    }

    // faz o link do vertex shader com o fragment shader
    this->_id = glCreateProgram();
    glAttachShader(this->_id, vertex);
    glAttachShader(this->_id, fragment);

    if(!gs_path.empty()) {
        glAttachShader(this->_id, geometry);
    }

    glLinkProgram(this->_id);
    getProgramLog(this->_id, "SHADER::PROGRAM::LINKING_SUCCESSFULLY", "ERROR::SHADER::PROGRAM::LINKING_FAILED");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if(!gs_path.empty()) {
        glDeleteShader(geometry);
    }

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

void Shader::setUniformBlockBinding(const std::string &name, const GLuint binding)
{
    glUniformBlockBinding(this->_id, glGetUniformBlockIndex(this->_id, name.c_str()), binding);
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

void Shader::setMatrix2fv(const std::string &name, const GLfloat *value)
{
    glUniformMatrix2fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMatrix3fv(const std::string &name, const GLfloat *value)
{
    glUniformMatrix3fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMatrix4fv(const std::string &name, const GLfloat *value)
{
    glUniformMatrix4fv(glGetUniformLocation(this->_id, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setUniform1fv(const std::string &name, const GLfloat *value)
{
    glUniform1fv(glGetUniformLocation(this->_id, name.c_str()), 1, value);
}

void Shader::setUniform2fv(const std::string &name, const GLfloat *value)
{
    glUniform2fv(glGetUniformLocation(this->_id, name.c_str()), 1, value);
}

void Shader::setUniform3fv(const std::string &name, const GLfloat *value)
{
    glUniform3fv(glGetUniformLocation(this->_id, name.c_str()), 1, value);
}

void Shader::setUniform4fv(const std::string &name, const GLfloat *value)
{
    glUniform4fv(glGetUniformLocation(this->_id, name.c_str()), 1, value);
}

void Shader::setUniform1i(const std::string &name, const GLint value)
{
    glUniform1i(glGetUniformLocation(this->_id, name.c_str()), value);
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

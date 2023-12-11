#ifndef RENDERTEXT_HPP
#define RENDERTEXT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <shader.hpp>
#include <exception.hpp>
#include <vbo.hpp>
#include <vao.hpp>

struct Character
{
    GLuint texture_id;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class RenderText
{
    public:
        RenderText(const std::string &, const float &, const float &, const unsigned &, const unsigned &);
        virtual ~RenderText();

        void setCanvas(const float &canvas_width, const float &canvas_height);

        glm::mat4 getProjectionMatrix() { return projection; }

        bool loadCharacter(FT_Face &face, unsigned int c);
        void draw(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color);

        std::map<char, Character> &getCharacters() { return characters; }

        float getCanvasWidth() { return canvas_width; }
        float getCanvasHeight() { return canvas_height; }

    protected:
        std::map<char, Character> characters;
        glm::mat4 projection;

        float canvas_width;
        float canvas_height;

        VBO vbo;
        VAO vao;

    private:
};

#endif // RENDERTEXT_HPP

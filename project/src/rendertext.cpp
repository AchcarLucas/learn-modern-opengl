#include "rendertext.hpp"

RenderText::RenderText(const std::string &file, const float &canvas_width, const float &canvas_height, const unsigned &font_width, const unsigned &font_height)
{
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) {
        throw Exception("ERROR::RENDERTEXT::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;

    if (FT_New_Face(ft, file.c_str(), 0, &face)) {
        throw Exception("ERROR::RENDERTEXT::FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, font_width, font_height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if(!loadCharacter(face, c)) continue;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // cria a matrix de projeção indicada no construtor
    setCanvas(canvas_width, canvas_height);

    vao.bind();
    vbo.bind();

    // vertices para desenhar o quad
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    // os dois primeiros floats é a posição e os 2 ultimos floats é a coordenada da textura
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

RenderText::~RenderText()
{
    for(const auto& character : characters) {
        glDeleteTextures(1, &character.second.texture_id);
    }
}

void RenderText::setCanvas(const float &canvas_width, const float &canvas_height)
{
    this->canvas_width = canvas_width;
    this->canvas_height = canvas_height;
    this->projection = glm::ortho(0.0f, canvas_width, 0.0f, canvas_height);
}

bool RenderText::loadCharacter(FT_Face &face, unsigned int c)
{
    // load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return false;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );

    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // now store character for later use
    Character character = {
        texture_id,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        (unsigned) face->glyph->advance.x
    };

    characters.insert(std::pair<char, Character>(c, character));

    return true;
}

void RenderText::renderText(Shader *shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    shader->use();
    shader->setUniform3fv("text_color", glm::value_ptr(color));
    shader->setMatrix4fv("projection", glm::value_ptr(getProjectionMatrix()));
    shader->setInt("text_sampler", 0);

    glActiveTexture(GL_TEXTURE0);

    vao.bind();

    for (auto &c : text) {
        Character ch = characters[c];

        float x_pos = x + ch.bearing.x * scale;
        float y_pos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // update VBO for each character
        float vertices[6][4] = {
            { x_pos,     y_pos + h,   0.0f, 0.0f },
            { x_pos,     y_pos,       0.0f, 1.0f },
            { x_pos + w, y_pos,       1.0f, 1.0f },

            { x_pos,     y_pos + h,   0.0f, 0.0f },
            { x_pos + w, y_pos,       1.0f, 1.0f },
            { x_pos + w, y_pos + h,   1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        vbo.bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        vbo.unbind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    vao.unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

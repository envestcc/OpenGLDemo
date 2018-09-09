#include"game.h"
#include"sprite_render.h"
#include"resource_manager.h"
#include"wdf.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>
#include<iostream>
#include"ft2build.h"
#include FT_FREETYPE_H  
#include <glm/gtc/type_ptr.hpp>


void Game::RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // 激活对应的渲染状态
    s.Use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // 遍历文本中所有的字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Game::Game(GLuint width, GLuint height):
    Width(width), Height(height), State(GAME_ACTIVE),
    wdf(nullptr), was(nullptr)
{
}

Game::~Game()
{
    if (render != nullptr)
        delete render;
    if (wdf != nullptr)
        delete wdf;
    // if (was != nullptr)
    //     delete was;
}

void Game::Init()
{
    // Load WDF
    wdf = new Wdf("textures/wzife.wd1");

    // Load Shaders
    ResourceManager::LoadShader("glsl/vsSprite.shader", "glsl/fsSprite.shader", nullptr, "sprite");
    // config shaders
    glm::mat4  projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    
    // Load textures
    ResourceManager::LoadTexture("textures/sasuke.jpg", GL_FALSE, "face");
    render = new SpriteRender(ResourceManager::GetShader("sprite"));

    // Fonts
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    FT_Face face;
    if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(GLubyte c=0;c<128;c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    Shader shader = ResourceManager::LoadShader("glsl/vsfont.shader", "glsl/fsfont.shader", nullptr, "font");
    glm::mat4 projectionFont = glm::ortho(0.0f, static_cast<GLfloat>(Width), 0.0f, static_cast<GLfloat>(Height));
    shader.Use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionFont));
}

void Game::ProcessInput(GLfloat dt)
{
    static GLfloat tt = 0;
    tt += dt;
    if (tt < 0.2) return;
    if (Keys[GLFW_KEY_N] == GL_TRUE)
    {
        tt = 0;
        static int id = 0;
        id = (id+1) % wdf->wasHeaderIndexs.size();
        uint32_t uuid = wdf->wasHeaderIndexs[id].uid;
        std::cout << id << " " << uuid << std::endl;
        was = wdf->LoadWas(uuid);
        if (was == nullptr)
            return;
        frameIndex = 0;
        char name[32] = "";
        sprintf(name, "test");
        ResourceManager::LoadTexture(was->frameHeaders[frameIndex].width, was->frameHeaders[frameIndex].height, (unsigned char*)was->frames[frameIndex].data(), name);
    }else if (Keys[GLFW_KEY_M] == GL_TRUE && was)
    {
        tt = 0;
        frameIndex = (frameIndex+1) % (was->header.imgDirCnt*was->header.imgFrameCnt);
        std::cout << frameIndex << std::endl;
        char name[32] = "";
        sprintf(name, "test");
        ResourceManager::LoadTexture(was->frameHeaders[frameIndex].width, was->frameHeaders[frameIndex].height, (unsigned char*)was->frames[frameIndex].data(), name);
    }
}

void Game::Update(GLfloat dt)
{

}

void Game::Render()
{
    static float rotate = 0.0f;
    // rotate += 0.1f;
    if (was)
    {
        char name[32] = "";
        sprintf(name, "test", frameIndex);
        int width = 400;
        int height = was->frameHeaders[frameIndex].height/(float)(was->frameHeaders[frameIndex].width) * width;
        render->DrawSprite(ResourceManager::GetTexture(name), glm::vec2(0, 0), glm::vec2(width, height), rotate, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    RenderText(ResourceManager::GetShader("font"), "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8, 0.2));
}
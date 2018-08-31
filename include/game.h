/****************************************************************

Game Manager.

Author: cc

****************************************************************/

#ifndef GAME_H
#define GAME_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "sprite_render.h"
#include"wdf.h"

enum GameState {
    GAME_ACTIVE
};


struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Game
{
public:
    GameState State;
    GLboolean Keys[1024];
    GLuint Width, Height;
    SpriteRender *render;
    Wdf *wdf;
    Was *was;
    int frameIndex;
    std::map<GLchar, Character> Characters;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
    GLuint VAO;
    GLuint VBO;
};



#endif
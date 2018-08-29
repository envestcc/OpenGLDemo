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


class Game
{
public:
    GameState State;
    GLboolean Keys[1024];
    GLuint Width, Height;
    SpriteRender *render;
    Wdf *wdf;
    Was *was;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
};


#endif
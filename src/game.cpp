#include"game.h"
#include"sprite_render.h"
#include"resource_manager.h"
#include"wdf.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>

Game::Game(GLuint width, GLuint height):
    Width(width), Height(height), State(GAME_ACTIVE)
{
}

Game::~Game()
{
    delete render;
}

void Game::Init()
{
    // Load Shaders
    ResourceManager::LoadShader("glsl/vsSprite.shader", "glsl/fsSprite.shader", nullptr, "sprite");
    // config shaders
    glm::mat4  projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("textures/sasuke.jpg", GL_FALSE, "face");
    render = new SpriteRender(ResourceManager::GetShader("sprite"));

    // Load WDF
    Wdf wdf("textures/goods.wdf");

}

void Game::ProcessInput(GLfloat dt)
{

}

void Game::Update(GLfloat dt)
{

}

void Game::Render()
{
    static float rotate = 0.0f;
    rotate += 0.1f;
    render->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(480, 340), rotate, glm::vec3(1.0f, 1.0f, 1.0f));
}
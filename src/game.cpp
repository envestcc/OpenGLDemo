#include"game.h"
#include"sprite_render.h"
#include"resource_manager.h"
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
    ResourceManager::LoadTexture("textures/sasuke.jpg", GL_TRUE, "face");

    render = new SpriteRender(ResourceManager::GetShader("sprite"));

}

void Game::ProcessInput(GLfloat dt)
{

}

void Game::Update(GLfloat dt)
{

}

void Game::Render()
{
    render->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}
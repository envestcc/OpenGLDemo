#include"game.h"
#include"sprite_render.h"
#include"resource_manager.h"
#include"wdf.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>

Game::Game(GLuint width, GLuint height):
    Width(width), Height(height), State(GAME_ACTIVE),
    wdf(nullptr), was(nullptr)
{
}

Game::~Game()
{
    delete render;
}

void Game::Init()
{
    // Load WDF
    wdf = new Wdf("textures/goods.wdf");

    // Load Shaders
    ResourceManager::LoadShader("glsl/vsSprite.shader", "glsl/fsSprite.shader", nullptr, "sprite");
    // config shaders
    glm::mat4  projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    
    // Load textures
    ResourceManager::LoadTexture("textures/sasuke.jpg", GL_FALSE, "face");
    render = new SpriteRender(ResourceManager::GetShader("sprite"));

}

void Game::ProcessInput(GLfloat dt)
{
    static GLfloat tt = 0;
    tt += dt;
    if (tt < 0.2) return;
    if (Keys[GLFW_KEY_N] == GL_TRUE)
    {
        tt = 0;
        if (was != nullptr)
            delete was;
        static int id = 0;
        id = (id+1) % wdf->wasHeaderIndexs.size();
        uint32_t uuid = wdf->wasHeaderIndexs[id]->uid;
        was = wdf->LoadWas(uuid);
        ResourceManager::LoadTexture(was->header.imgWidth, was->header.imgHeight, (unsigned char*)was->frames[0].data(), "test");
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
        render->DrawSprite(ResourceManager::GetTexture("test"), glm::vec2(200, 200), glm::vec2(was->header.imgWidth, was->header.imgHeight), rotate, glm::vec3(1.0f, 1.0f, 1.0f));
}
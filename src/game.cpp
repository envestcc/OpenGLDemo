#include"game.h"
#include"sprite_render.h"
#include"resource_manager.h"
#include"wdf.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glad/glad.h>
#include<iostream>

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
        render->DrawSprite(ResourceManager::GetTexture(name), glm::vec2(200, 200), glm::vec2(was->frameHeaders[frameIndex].width, was->frameHeaders[frameIndex].height), rotate, glm::vec3(1.0f, 1.0f, 1.0f));
    }
        
}
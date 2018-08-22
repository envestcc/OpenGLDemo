
//
//  main.cpp
//  openglDemo
//
//  Created by chen on 2018/4/5.
//  Copyright © 2018年 chen. All rights reserved.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "resource_manager.h"
#include "game.h"


void framebuffer_size_callback(GLFWwindow* win, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //创建窗口
    GLFWwindow* win = glfwCreateWindow(800, 600, "OpenglDemo", nullptr, nullptr);
    if (nullptr == win) {
        std::cout << "failed create window.";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }
    //设置回调
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    glfwSetKeyCallback(win, key_callback);

    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize game
    game.Init();

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // game state
    game.State = GAME_ACTIVE;
    
    while (!glfwWindowShouldClose(win)) {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //检查并调用事件
        glfwPollEvents();

        // Manage user input
        game.ProcessInput(deltaTime);
        
        // Update game state
        game.Update(deltaTime);

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game.Render();
        
        //交换缓冲
        glfwSwapBuffers(win);
    }
    
    glfwTerminate();

    return 0;
}

//窗口尺寸改变
void framebuffer_size_callback(GLFWwindow* win, int width, int height){
    glViewport(0,0,width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user process the escape key, we set the WindowShouldClose property to true, closing the app.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            game.Keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            game.Keys[key] = GL_FALSE;
    }
}


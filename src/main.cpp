
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

void processInput(GLFWwindow* win);
void render();
void framebuffer_size_callback(GLFWwindow* win, int width, int height);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
    "}";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";


int main(int argc, const char * argv[]) {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //创建窗口
    GLFWwindow* win = glfwCreateWindow(800, 600, "OpenglDemo", NULL, NULL);
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
    //设置尺寸修改回调
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    
    Shader myShader = ResourceManager::LoadShader("glsl/vertex.shader", "glsl/fragment.shader", nullptr, "simple");
    
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    unsigned int indices[] = { // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    //生成缓冲对象
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    //绑定新创建的缓冲到gl_array_buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //把数据传到缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
    
    
    while (!glfwWindowShouldClose(win)) {
        processInput(win);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        myShader.Use();
        glBindVertexArray(VAO);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //交换缓冲
        glfwSwapBuffers(win);
        //检查并调用事件
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

//处理输入
void processInput(GLFWwindow* win){
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }
}

//渲染
void render(){

    
    
   
}

//窗口尺寸改变
void framebuffer_size_callback(GLFWwindow* win, int width, int height){
    glViewport(0,0,width, height);
}


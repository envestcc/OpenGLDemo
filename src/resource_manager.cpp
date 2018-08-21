#include<fstream>
#include<sstream>
#include<iostream>
#include"resource_manager.h"

std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Shader ResourceManager::LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    std::ifstream inFile;
    std::stringstream vShaderStream, fShaderStream, gShaderStream;
    std::string vShader, fShader, gShader;
    inFile.open(vShaderFile);
    vShaderStream << inFile.rdbuf();
    inFile.close();
    vShader = vShaderStream.str();
    inFile.open(fShaderFile);
    fShaderStream << inFile.rdbuf();
    inFile.close();
    if (gShaderFile != nullptr)
    {
        inFile.open(gShaderFile);
        gShaderStream << inFile.rdbuf();
        gShader = gShaderStream.str();
        inFile.close();
    }
    fShader = fShaderStream.str();

    Shader shader;
    shader.Compile(vShader.c_str(), fShader.c_str(), gShaderFile != nullptr ? gShader.c_str() : nullptr);
    return shader;
}
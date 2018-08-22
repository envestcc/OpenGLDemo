#include<fstream>
#include<sstream>
#include<iostream>
#include"resource_manager.h"
#include"stb_image.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;

Shader& ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
    Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Shader& ResourceManager::LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
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

Texture2D& ResourceManager::LoadTexture(const GLchar* file, GLboolean alpha, std::string name)
{
    Textures[name] = LoadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

Texture2D& ResourceManager::LoadTextureFromFile(const GLchar* file, GLboolean alpha)
{
    // Create Texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // Load image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
    // Now generate texture
    texture.Generate(width, height, data);
    // free image data
    stbi_image_free(data);
    return texture;

}
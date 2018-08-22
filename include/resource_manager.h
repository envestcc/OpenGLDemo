/****************************************************************

Resource Manager

Author: cc

****************************************************************/


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include<string>
#include<map>
#include"shader.h"
#include"texture.h"

class ResourceManager 
{
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;

    static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);

    static Shader GetShader(std::string name);

    static Texture2D LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

    static Texture2D GetTexture(std::string name);

    static void Clear();

private:
    ResourceManager(){}

    static Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

    static Texture2D LoadTextureFromFile(const GLchar* file, GLboolean alpha);

};



#endif
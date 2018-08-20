/****************************************************************

Resource Manager

Author: cc

****************************************************************/


#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include<string>
#include<map>
#include"shader.h"

class ResourceManager 
{
public:
    static std::map<std::string, Shader> Shaders;

    static Shader LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);

    static Shader GetShader(std::string name);

private:
    ResourceManager(){}

    static Shader LoadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

};



#endif
/****************************************************************

Widget System

Author: cc

****************************************************************/


#ifndef WIDGET_H
#define WIDGET_H

#include<vector>
#include<string>
#include<glm/glm.hpp>
#include"sprite_render.h"
#include"texture.h"

namespace Gui 
{

class Widget
{
private:
    float x;
    float y;
    float width;
    float height;
    std::string name;
    SpriteRender* render;
    Texture2D* texture;
    std::vector<Widget*> children;
    bool enable;

public:
    Widget();
    virtual ~Widget();
    virtual void Update(GLfloat dt);
    virtual void Render();
    virtual void onKeyUp();
    virtual void onMouseClick();

};

}



#endif
/****************************************************************

Sprite Render

Author: cc

****************************************************************/


#ifndef SPRITERENDER_H
#define SPRITERENDER_H

#include<glm/glm.hpp>
#include "shader.h"
#include "texture.h"

class SpriteRender
{
public:
    // Constructor
    SpriteRender(Shader &shader);
    // Destructor
    ~SpriteRender();
    // Render a defined quad textured with given sprite
    void DrawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color=glm::vec3(1.0f));
private:
    // Render State
    Shader shader;
    GLuint quadVAO;
    // Initialize and configures the quad's buffer and vertex attributes
    void initRenderData();

    
};


#endif
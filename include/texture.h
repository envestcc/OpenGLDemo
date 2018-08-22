/****************************************************************

Texture Manager

Author: cc

****************************************************************/

#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>

class Texture2D
{
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texutre
    GLuint ID;
    // Texture image dimensions
    GLuint Width, Height; // Width and height of loaded image in pixels
    // Texture Format
    GLuint Internal_Format; // Format of texture object
    GLuint Image_Format;    // Format of loaded image
    // Texture configuration
    GLuint Wrap_S;  // Wrapping model on S axis
    GLuint Wrap_T;  // Wrapping model on T axis
    GLuint Filter_Min;  // Filtering mode if texture pixels < screen pixels
    GLuint Filter_Max;  // Filtering mode if texture pixels > screen pixels
    // Constructor (sets default texture mode)
    Texture2D();
    // Generates texture from image data
    void Generate(GLuint width, GLuint height, unsigned char* data);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

#endif
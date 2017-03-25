#pragma once

#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

class ImageLoader
{
public:
	virtual ~ImageLoader() {}
	virtual GLuint loadTextureRGB( GLchar* path ) = 0;
	virtual GLuint loadTextureRGBA( GLchar* path ) = 0;
	virtual GLuint loadCubemap( std::vector<const GLchar*> faces ) = 0;
	virtual void setFlipOnLoad( GLboolean flip ) = 0;
};
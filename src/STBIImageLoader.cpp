#include "STBIImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../OpenGL libs/stb_image.h"

#include <iostream>

GLuint STBIImageLoader::loadTextureRGB( GLchar * path )
{
	//Generate a textureID
	GLuint textureID;
	
	//Generate a texture
	glGenTextures( 1, &textureID );
	
	int width, height;
	unsigned char* loadedImage = stbi_load( path, &width, &height, 0, STBI_rgb );

	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, loadedImage );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );
	stbi_image_free( loadedImage );
	return textureID;
}

GLuint STBIImageLoader::loadTextureRGBA( GLchar * path )
{
	//Generate a textureID
	GLuint textureID;

	//Generate a texture
	glGenTextures( 1, &textureID );

	int width, height;
	unsigned char* loadedImage = stbi_load( path, &width, &height, 0, STBI_rgb_alpha );

	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedImage );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );
	stbi_image_free( loadedImage );
	return textureID;
}

GLuint STBIImageLoader::loadCubemap( std::vector<const GLchar*> faces )
{
	return GLuint();
}

void STBIImageLoader::setFlipOnLoad( GLboolean b )
{
	flipOnLoad = b;
}

bool STBIImageLoader::getFlipOnLoad()
{
	return flipOnLoad;
}

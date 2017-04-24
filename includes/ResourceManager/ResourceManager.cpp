#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Texture2D>    ResourceManager::textures;

Texture2D ResourceManager::loadTexture( const GLchar *file, GLboolean RGBA, std::string name )
{
	Texture2D texture;
	if (RGBA)
	{
		texture.format = GL_RGBA;
	}
	int width, height;
	unsigned char* image = stbi_load( file, &width, &height, 0, texture.format == GL_RGBA ? STBI_rgb_alpha : STBI_rgb );
	texture.generate( width, height, image );
	stbi_image_free( image );
	textures[name] = texture;
	return texture;
}


Texture2D ResourceManager::getTexture( std::string name )
{
	return textures[name];
}


void ResourceManager::clear()
{
	for (auto iter : textures)
		glDeleteTextures( 1, &iter.second.textureID );
	textures.clear();
}

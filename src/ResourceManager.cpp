#include "../includes/ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>       ResourceManager::shaders;

Shader ResourceManager::loadShader( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile, std::string name )
{
	std::string vertCode;
	std::string fragCode;
	std::string geomCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::ifstream geomShaderFile;
	vertexShaderFile.exceptions( std::ifstream::badbit );
	fragmentShaderFile.exceptions( std::ifstream::badbit );
	geomShaderFile.exceptions( std::ifstream::badbit );
	try
	{
		vertexShaderFile.open( vertFile );
		fragmentShaderFile.open( fragFile );
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertCode = vShaderStream.str();
		fragCode = fShaderStream.str();
		if (geomFile != nullptr)
		{
			geomShaderFile.open( geomFile );
			std::stringstream gShaderStream;
			gShaderStream << geomShaderFile.rdbuf();
			geomShaderFile.close();
			geomCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "SHADER ERROR: FAILED TO READ SHADER FILE(S)" << std::endl;
	}
	const GLchar *vertShaderCode = vertCode.c_str();
	const GLchar *fragShaderCode = fragCode.c_str();
	const GLchar *geomShaderCode = geomCode.c_str();
	Shader shader;
	shader.compile( vertShaderCode, fragShaderCode, geomFile != nullptr ? fragShaderCode : nullptr );
	shaders[name] = shader;
	return shaders[name];
}

Shader ResourceManager::getShader( std::string name )
{
	return shaders[name];
}

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

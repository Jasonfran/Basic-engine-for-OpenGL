#include "../includes/ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>       ResourceManager::shaders;
std::map<std::string, Framebuffer> ResourceManager::framebuffers;

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
	shader.vertPath = vertFile;
	shader.fragPath = fragFile;
	shader.geomPath = geomFile;
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

void ResourceManager::reloadShaders()
{
	for (auto &s : shaders)
	{	
		glUseProgram( 0 );
		glDeleteProgram( s.second.shaderID );
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
			vertexShaderFile.open( s.second.vertPath );
			fragmentShaderFile.open( s.second.fragPath );
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
			vertexShaderFile.close();
			fragmentShaderFile.close();
			vertCode = vShaderStream.str();
			fragCode = fShaderStream.str();
			if (s.second.geomPath != nullptr)
			{
				geomShaderFile.open( s.second.geomPath );
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
		s.second.compile( vertShaderCode, fragShaderCode, s.second.geomPath != nullptr ? fragShaderCode : nullptr );
	}
	std::cout << "Shaders reloaded" << std::endl;
}

Texture2D ResourceManager::getTexture( std::string name )
{
	return textures[name];
}

GLuint ResourceManager::createFramebuffer( std::string name, GLuint width, GLuint height, GLboolean addDepth)
{
	GLuint fbo;
	glGenFramebuffers( 1, &fbo );
	std::cout << "Generated FBO: " << fbo << std::endl;
	Framebuffer framebuffer;
	framebuffer.fbo = fbo;
	framebuffer.width = width;
	framebuffer.height = height;
	
	Texture2D depthTexture;
	if (addDepth)
	{
		depthTexture.generate( framebuffer.width, framebuffer.height, NULL, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST );
		glBindFramebuffer( GL_FRAMEBUFFER, framebuffer.fbo );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture.textureID, 0 );
	}
	GLuint status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete! " << status << std::endl;
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
	if (addDepth)
		framebuffer.depthTexture = depthTexture.textureID;
	framebuffers[name] = framebuffer;
	return framebuffer.fbo;
}

void ResourceManager::addColourTextureToFramebuffer(std::string fboName, std::string textureName, GLuint internalFormat, GLuint format, GLuint dataType )
{
	Framebuffer &framebuffer = framebuffers[fboName];
	Texture2D texture;
	texture.generate( framebuffer.width, framebuffer.height, NULL, internalFormat, format, dataType );
	glBindFramebuffer( GL_FRAMEBUFFER, framebuffer.fbo );
	framebuffer.colourTextures[textureName] = texture.textureID;
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + framebuffer.colourTextures.size()-1, GL_TEXTURE_2D, texture.textureID, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


void ResourceManager::clear()
{
	for (auto iter : shaders)
		glDeleteProgram( iter.second.shaderID );
	for (auto iter : textures)
		glDeleteTextures( 1, &iter.second.textureID );
}

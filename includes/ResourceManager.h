#pragma once
#include <GL/glew.h>
#include <string>
#include <map>

#include "Texture2D.h"
#include "Shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader>    shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader loadShader( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile, std::string name );
	static Shader getShader( std::string name );

	static Texture2D loadTexture( const GLchar* file, GLboolean RGBA, std::string name );
	static Texture2D getTexture( std::string name );

	static void clear();
private:
	ResourceManager() {}
};
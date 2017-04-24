#pragma once
#include <GL/glew.h>
#include <string>
#include <map>

#include "../Texture2D/Texture2D.h"
#include "../Shader/Shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Texture2D> textures;

	static Texture2D loadTexture( const GLchar* file, GLboolean RGBA, std::string name );
	static Texture2D getTexture( std::string name );

	static void clear();
private:
	ResourceManager() {}
};
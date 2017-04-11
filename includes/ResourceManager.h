#pragma once
#include <GL/glew.h>
#include <string>
#include <map>

#include "Texture2D.h"
#include "Shader.h"
#include "Framebuffer.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader>    shaders;
	static std::map<std::string, Texture2D> textures;
	static std::map<std::string, Framebuffer> framebuffers;

	static Shader loadShader( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile, std::string name );
	static Shader getShader( std::string name );

	static Texture2D loadTexture( const GLchar* file, GLboolean RGBA, std::string name );
	static void reloadShaders();
	static Texture2D getTexture( std::string name );

	static GLuint createFramebuffer( std::string name, GLuint width, GLuint height, GLboolean addDepth = true, GLboolean forShadow = false );
	static void addColourTextureToFramebuffer( std::string fboName, std::string textureName, GLuint internalFormat, GLuint format, GLuint dataType );
	static Framebuffer getFramebuffer( std::string name ) { return framebuffers[name]; }

	static void clear();
private:
	ResourceManager() {}
};
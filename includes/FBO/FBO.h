#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

class FBO
{
public:
	GLuint id;
	// name and texture id
	std::map<std::string, GLuint> colourTextures;
	GLuint depthTexture;
	//GLuint attachedStencilTexture; Will add when I need it
	GLuint bind();
	GLuint createColour(std::string name, GLuint target, GLuint level, GLuint internalFormat, GLuint width, GLuint height, GLuint border, GLuint format, GLuint type, GLvoid * data, GLuint minF = GL_NEAREST, GLuint magF = GL_NEAREST, GLuint wrapS = GL_REPEAT, GLuint wrapT = GL_REPEAT );
	GLuint createDepth(GLuint target, GLuint level, GLuint width, GLuint height, GLuint type);
	GLuint createDepthCubemap( GLuint level, GLuint width, GLuint height, GLuint type );
	GLuint attachColour(std::string name, GLuint id);
	GLuint attachDepth(GLuint id);
	FBO();
	~FBO();
};
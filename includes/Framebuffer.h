#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
class Framebuffer
{
public:
	GLuint fbo;
	GLuint rbo;
	GLuint width;
	GLuint height;
	std::map<std::string, GLuint> colourTextures;
	GLuint depthTexture;
	void bind() { glBindFramebuffer( GL_FRAMEBUFFER, fbo ); }
	GLuint getColourTexture( std::string name ) { return colourTextures[name]; }
};
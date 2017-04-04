#pragma once
#include <GL/glew.h>

class Texture2D
{
public:
	GLuint textureID;
	GLuint width, height;
	GLuint internalFormat;
	GLuint format;
	GLuint dataType;
	GLuint wrapS;
	GLuint wrapT;
	GLuint filterMin;
	GLuint filterMax;
	Texture2D();
	void generate( GLuint width, GLuint height, unsigned char * data, GLuint internalFormat = GL_RGB, GLuint format = GL_RGB, GLuint dataType = GL_UNSIGNED_BYTE,
				   GLuint wrapS = GL_REPEAT, GLuint wrapT = GL_REPEAT, GLuint filterMin = GL_LINEAR, GLuint filterMax = GL_LINEAR );
	void bind() const;
};
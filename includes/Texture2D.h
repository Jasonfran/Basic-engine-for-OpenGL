#pragma once
#include <GL/glew.h>

class Texture2D
{
public:
	GLuint textureID;
	GLuint width, height;
	GLuint format;
	GLuint wrapS;
	GLuint wrapT;
	GLuint filterMin;
	GLuint filterMax;
	Texture2D();
	void generate( GLuint width, GLuint height, unsigned char *data );
	void bind() const;
};
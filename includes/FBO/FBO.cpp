#include "FBO.h"
#include <iostream>

GLuint FBO::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	return this->id;
}

GLuint FBO::createColour( std::string name, GLuint target, GLuint level, GLuint internalFormat, GLuint width, GLuint height, GLuint border, GLuint format, GLuint type, GLvoid * data, GLuint minF, GLuint magF, GLuint wrapS, GLuint wrapT )
{
	GLuint textureID;
	glGenTextures( 1, &textureID );
	glBindTexture( target, textureID );
	glTexImage2D( target, level, internalFormat, width, height, border, format, type, data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minF );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magF );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT );
	glBindTexture( target, 0 );
	this->colourTextures[name] = textureID;
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colourTextures.size() - 1, target, textureID, level );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	return textureID;
}

GLuint FBO::createDepth( GLuint target, GLuint level, GLuint width, GLuint height, GLuint type )
{
	GLuint textureID;
	glGenTextures( 1, &textureID );
	glBindTexture( target, textureID );
	glTexImage2D( target, level, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, type, NULL );

	glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv( target, GL_TEXTURE_BORDER_COLOR, borderColor );
	glBindTexture( target, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, textureID, level );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	this->depthTexture = textureID;
	return textureID;
}

GLuint FBO::createDepthCubemap( GLuint level, GLuint width, GLuint height, GLuint type )
{
	GLuint cubemap;
	glGenTextures( 1, &cubemap );

	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap );
	for (GLuint i = 0; i < 6; i++)
	{
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, type, NULL );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	}
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubemap, level);
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	this->depthTexture = cubemap;
	return cubemap;
}

GLuint FBO::attachColour( std::string name, GLuint id )
{
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->colourTextures.size() - 1, id, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	this->colourTextures[name] = id;
	return id;
}

GLuint FBO::attachDepth(GLuint id)
{
	glBindFramebuffer( GL_FRAMEBUFFER, this->id );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	return id;
}

FBO::FBO()
{
	glGenFramebuffers( 1, &this->id );
	std::cout << "Generated FBO with id: " << this->id << std::endl;
}

FBO::~FBO()
{
	glDeleteFramebuffers( 1, &this->id );
}
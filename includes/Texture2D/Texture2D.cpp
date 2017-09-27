#include "Texture2D.h"
#include "../stb_image.h"

Texture2D::Texture2D()
{
	glGenTextures( 1, &this->textureID );
}

void Texture2D::generate( GLuint width, GLuint height, unsigned char * data, GLuint internalFormat, GLuint format, GLuint dataType, 
						  GLuint wrapS, GLuint wrapT, GLuint filterMin, GLuint filterMax )
{
	this->width = width;
	this->height = height;
	this->internalFormat = internalFormat;
	this->format = format;
	this->dataType = dataType;
	this->wrapS = wrapS;
	this->wrapT = wrapT;
	this->filterMin = filterMin;
	this->filterMax = filterMax;

	glBindTexture( GL_TEXTURE_2D, this->textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->dataType, data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::genFromFile( char* path )
{
	int width, height;
	unsigned char* image = stbi_load( path, &width, &height, 0, STBI_rgb );
	// Assign texture to ID
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
	glGenerateMipmap( GL_TEXTURE_2D );

	// Parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glBindTexture( GL_TEXTURE_2D, 0 );
	stbi_image_free( image );
}

void Texture2D::bind() const
{
	glBindTexture( GL_TEXTURE_2D, this->textureID );
}

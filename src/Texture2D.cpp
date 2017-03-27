#include "..\includes\Texture2D.h"

Texture2D::Texture2D(): width(0), height(0), format(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR)
{
	glGenTextures( 1, &this->textureID );
}

void Texture2D::generate( GLuint width, GLuint height, unsigned char * data )
{
	this->width = width;
	this->height = height;
	
	glBindTexture( GL_TEXTURE_2D, this->textureID );
	glTexImage2D( GL_TEXTURE_2D, 0, this->format, width, height, 0, this->format, GL_UNSIGNED_BYTE, data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax );

	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::bind() const
{
	glBindTexture( GL_TEXTURE_2D, this->textureID );
}

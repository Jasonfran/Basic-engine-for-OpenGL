#pragma once

#include "ImageLoader.h"

class STBIImageLoader : public ImageLoader
{
public:
	virtual GLuint loadTextureRGB( GLchar * path ) override;
	virtual GLuint loadTextureRGBA( GLchar * path ) override;
	virtual GLuint loadCubemap( std::vector<const GLchar*> faces ) override;

	void setFlipOnLoad( GLboolean flip );
	bool getFlipOnLoad();
private:
	bool flipOnLoad = false;
};
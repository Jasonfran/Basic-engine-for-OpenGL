#pragma once
#include "ImageLoader.h"

class Manager
{
public:
	//Get and set image loader
	static void setImageLoader( ImageLoader* loader )
	{
		_imageLoader = loader;
	}

	static ImageLoader* getImageLoader()
	{
		return _imageLoader;
	}


private:
	static ImageLoader* _imageLoader;
};
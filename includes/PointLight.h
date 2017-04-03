#pragma once
#include "../includes/SceneObject.h"

class PointLight : public SceneObject
{
public:
	PointLight() {}
	void draw( Shader shader );
};
#pragma once
#include <GL/glew.h>

class Scene
{
public:
	virtual void update(GLfloat delta) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;
};
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "../includes/SceneObject.h"
#include "../includes/Camera.h"
class Scene
{
public:
	GLboolean keys[1024];
	GLfloat delta;
	std::map <std::string, SceneObject*> sceneObjects;
	Camera camera;
	virtual void update(GLfloat delta) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;
	virtual void keyInput() = 0;
	virtual void mouseInput( double xoffset, double yoffset ) = 0;
	virtual void scrollInput( double xpos, double ypos ) = 0;
};
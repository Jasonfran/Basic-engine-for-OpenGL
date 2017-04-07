#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "SceneObject.h"
#include "Camera.h"
#include "PointLight.h"
class Scene
{
public:
	GLboolean keys[1024];
	std::map <std::string, SceneObject*> sceneObjects;
	std::map <std::string, PointLight*> sceneLights;
	Camera camera;
	virtual void update(GLfloat delta) = 0;
	virtual void updateShaderUniforms( GLfloat delta ) = 0;
	virtual void render() = 0;
	virtual void exit() = 0;
	virtual void keyInput(GLfloat delta) = 0;
	virtual void mouseInput( double xoffset, double yoffset ) = 0;
	virtual void scrollInput( double xpos, double ypos ) = 0;
};
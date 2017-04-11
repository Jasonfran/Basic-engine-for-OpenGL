#pragma once
#ifndef SCENE_H
#define SCENE_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneObject.h"
#include "PointLight.h"

class Scene
{
public:
	std::map <std::string, SceneObject*> sceneObjects;
	std::map <std::string, PointLight*> sceneLights;
	Camera camera;
	virtual void update(GLfloat delta) = 0;
	virtual void updateShaderUniforms( GLfloat delta ) = 0;
	virtual void render() = 0;
	virtual void keyInput(GLfloat delta) = 0;
	virtual void mouseButtonInput() = 0;
	virtual void mouseInput() = 0;
	virtual void scrollInput() = 0;
	virtual ~Scene() {
		for (std::map<std::string, SceneObject*>::iterator i = sceneObjects.begin(); i != sceneObjects.end(); i++)
		{
			delete i->second;
		}
		for (std::map<std::string, PointLight*>::iterator i = sceneLights.begin(); i != sceneLights.end(); i++)
		{
			delete i->second;
		}
		sceneObjects.clear();
		sceneLights.clear();
		ResourceManager::clear();
	}
};
#endif // !SCENE_H

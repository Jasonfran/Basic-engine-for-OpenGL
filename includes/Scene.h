#pragma once
#ifndef SCENE_H
#define SCENE_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include "Camera.h"
#include "PointLight/PointLight.h"
#include <memory>
#include "FBO/FBO.h"

class Scene
{
public:
	std::map <std::string, std::unique_ptr<SceneObject>> sceneObjects;
	std::map <std::string, std::unique_ptr<PointLight>> pointLights;
	std::map <std::string, std::unique_ptr<FBO>> framebuffers;
	std::map <std::string, std::unique_ptr<Shader>> shaders;
	Camera camera;
	virtual void update(GLfloat delta) = 0;
	virtual void updateShaderUniforms( GLfloat delta ) = 0;
	virtual void render() = 0;
	virtual void keyInput(GLfloat delta) = 0;
	virtual void mouseButtonInput() = 0;
	virtual void mouseInput() = 0;
	virtual void scrollInput() = 0;
	virtual ~Scene() { std::cout << "Exiting Scene" << std::endl; }
};
#endif // !SCENE_H

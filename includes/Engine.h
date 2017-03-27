#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../includes/ResourceManager.h"
#include "../includes/Scene.h"


class Engine
{
public:
	static Engine &instance();
	void init( GLuint width, GLuint height, GLchar* title );
	void update();
	void render();
	void changeScene(Scene *nextScene);
	GLFWwindow *getCurrentWindow() { return mainWindow; }
	Engine() {};
private:
	Scene *currentScene;
	GLFWwindow *mainWindow;
	GLuint SCREEN_WIDTH;
	GLuint SCREEN_HEIGHT;
	GLchar* WINDOW_TITLE;
	GLuint framerate = 60;
	void createWindow( GLuint width, GLuint height, GLchar* title );
};

#endif
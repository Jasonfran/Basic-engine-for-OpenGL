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
	static void keyInput( GLFWwindow * window, int key, int scancode, int action, int mode );
	static void mouseInput( GLFWwindow * window, double xoffset, double yoffset );
	static void scrollInput( GLFWwindow * window, double xpos, double ypos );
	GLuint SCREEN_WIDTH;
	GLuint SCREEN_HEIGHT;
	GLFWwindow *getCurrentWindow() { return mainWindow; }
private:
	Scene *currentScene;
	GLFWwindow *mainWindow;
	GLchar* WINDOW_TITLE;
	GLuint framerate = 60;
	void createWindow( GLuint width, GLuint height, GLchar* title );
	Engine() {};
	Engine( Engine const& );
	void operator=( Engine const& ); 
};

#endif
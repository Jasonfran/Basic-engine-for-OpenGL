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

// GL includes
#include "../../OpenGL libs/shader.h"
#include "../../OpenGL libs/camera.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Engine
{
public:
	static Engine &instance();
	void init();
	void createWindow( GLuint width, GLuint height, GLchar* title );
	void mainLoop();
	void update( GLfloat delta );
	void run();
private:
	GLFWwindow *mainWindow;
	GLuint SCREEN_WIDTH;
	GLuint SCREEN_HEIGHT;
	GLchar* WINDOW_TITLE;
	GLuint framerate = 60;
	Engine();
};

#endif
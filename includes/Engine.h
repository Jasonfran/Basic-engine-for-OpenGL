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

class Scene;
class Engine
{
public:
	static Engine &instance();
	void init( GLuint width, GLuint height, GLchar* title );
	void update();
	void render();
	void changeScene(Scene *nextScene);
	void globalKeyInput();
	GLuint SCREEN_WIDTH;
	GLuint SCREEN_HEIGHT;
	GLFWwindow *getCurrentWindow() { return mainWindow; }
	Scene *currentScene;
private:
	GLFWwindow *mainWindow;
	GLchar* WINDOW_TITLE;
	GLuint framerate = 60;
	void createWindow( GLuint width, GLuint height, GLchar* title );
	Engine() {};
	Engine( Engine const& );

};

#endif
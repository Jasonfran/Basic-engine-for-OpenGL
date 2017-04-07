
#include "../includes/Engine.h"
#include "../includes/BasicScene.h"
#include "../includes/ResourceManager.h"
#include <string>
#include <iostream>

Engine &Engine::instance()
{
	static Engine myInstance;
	return myInstance;
}

void Engine::init( GLuint width, GLuint height, GLchar* title )
{

	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
	
	createWindow( width, height, title );

	glewExperimental = GL_TRUE;
	glewInit();


	update();
}

void Engine::createWindow( GLuint width, GLuint height, GLchar* title )
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	WINDOW_TITLE = title;
	mainWindow = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "", nullptr, nullptr );
	glfwSetWindowTitle( mainWindow, WINDOW_TITLE );
	glfwMakeContextCurrent( mainWindow );
	glfwSwapInterval( 1 );
	glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	glfwSetKeyCallback( mainWindow, keyInput );
	glfwSetCursorPosCallback( mainWindow, mouseInput );
	glfwSetScrollCallback( mainWindow, scrollInput );

	glEnable( GL_DEPTH_TEST );
}

void Engine::update()
{
	if (currentScene == nullptr)
	{
		currentScene = new BasicScene();
	}
	GLfloat delta = 0.0f;
	GLfloat lastframetime = 0.0f;
	while (!glfwWindowShouldClose( mainWindow ))
	{
		GLfloat gametime = glfwGetTime();
		delta = gametime - lastframetime;
		lastframetime = gametime;
		//std::cout << delta << std::endl;

		glfwPollEvents();
		currentScene->update(delta);
		render();

	}
	ResourceManager::clear();
	glfwTerminate();
}

void Engine::render()
{
	currentScene->render();
	//std::cout << "I'm pushing the final image to the screen!" << std::endl;
	glfwSwapBuffers( mainWindow );
}

void Engine::changeScene( Scene *nextScene )
{
	currentScene->exit();
	currentScene = nextScene;
}

// This is a bit messy I think. But it works exactly how I want it to work. These need to be static.
void Engine::keyInput( GLFWwindow * window, int key, int scancode, int action, int mode )
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose( window, true );
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		ResourceManager::reloadShaders();
	}

	if (action == GLFW_PRESS)
		Engine::instance().currentScene->keys[key] = true;
	else if (action == GLFW_RELEASE)
		Engine::instance().currentScene->keys[key] = false;
}

void Engine::mouseInput( GLFWwindow * window, double xoffset, double yoffset )
{
	Engine::instance().currentScene->mouseInput( xoffset, yoffset );
}

void Engine::scrollInput( GLFWwindow * window, double xpos, double ypos )
{
	Engine::instance().currentScene->scrollInput( xpos, ypos );
}

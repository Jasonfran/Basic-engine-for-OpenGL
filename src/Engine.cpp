
#include "../includes/Engine.h"
#include "../includes/BasicScene.h"
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
		std::cout << delta << std::endl;

		currentScene->update(delta);
		render();

	}
	glfwTerminate();
}

void Engine::render()
{
	glfwPollEvents();
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	currentScene->render();
	std::cout << "I'm pushing the final image to the screen!" << std::endl;
	glfwSwapBuffers( mainWindow );
}

void Engine::changeScene( Scene *nextScene )
{
	currentScene = nextScene;
}

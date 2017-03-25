
#include "../headers/Engine.h"

#include <string>
#include "../headers/Manager.h"
#include "../headers/STBIImageLoader.h"

Engine &Engine::instance()
{
	static Engine myInstance;
	return myInstance;
}

void Engine::init()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

	glewExperimental = GL_TRUE;
	glewInit();
	
	STBIImageLoader *stbiLoader = new STBIImageLoader();
	Manager::setImageLoader( stbiLoader );

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

void Engine::mainLoop()
{
	GLfloat delta = 0.0f;
	GLfloat lastframetime = 0.0f;
	while (!glfwWindowShouldClose( mainWindow ))
	{
		GLfloat gametime = glfwGetTime();
		delta = gametime - lastframetime;
		lastframetime = gametime;
		std::cout << delta << std::endl;

		update( delta );

	}
	glfwTerminate();
}

void Engine::update( GLfloat delta )
{
	glfwPollEvents();
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glfwSwapBuffers( mainWindow );
}

void Engine::run()
{
	mainLoop();
}

Engine::Engine()
{
}



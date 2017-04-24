
#include "Engine.h"
#include "../../scenes/BasicScene.h"
#include "../Input/Input.h"
#include <string>
#include <iostream>
#include "../ResourceManager/ResourceManager.h"
#include "../Scene.h"
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

	glfwSetKeyCallback( mainWindow, Input::updateKey );
	glfwSetMouseButtonCallback( mainWindow, Input::updateMouseButton);
	glfwSetCursorPosCallback( mainWindow, Input::mouseInput );
	glfwSetScrollCallback( mainWindow, Input::scrollInput );
	glfwWindowHint( GLFW_SAMPLES, 4 );
	glEnable( GL_DEPTH_TEST );
}

void Engine::update()
{
	if (currentScene == nullptr)
	{
		currentScene = std::unique_ptr<BasicScene>(new BasicScene());
	}
	GLfloat delta = 0.0f;
	GLfloat lastframetime = 0.0f;
	while (!glfwWindowShouldClose( mainWindow ))
	{
		GLfloat gametime = glfwGetTime();
		delta = gametime - lastframetime;
		lastframetime = gametime;
		//std::cout << delta << std::endl;

		Input::updateStates();
		glfwPollEvents();
		globalKeyInput();
		currentScene->keyInput(delta);
		currentScene->mouseButtonInput();
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

void Engine::changeScene( Scene* nextScene )
{
	currentScene = std::unique_ptr<Scene>(nextScene);
}

void Engine::globalKeyInput()
{

	if (Input::keyWasPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose( mainWindow, true );
	}

	if (Input::keyWasPressed(GLFW_KEY_R))
	{
		for (auto &s : currentScene->shaders)
		{
			s.second->reload();
		}
		std::cout << "Shaders reloaded" << std::endl;
	}

	if (Input::keyWasPressed( GLFW_KEY_TAB ))
	{
		//currentScene->exit();
		currentScene.reset();
		changeScene(new BasicScene());
	}
}

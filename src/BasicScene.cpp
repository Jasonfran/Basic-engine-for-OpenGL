#include "../includes/Scene.h"
#include <iostream>
#include "../includes/BasicScene.h"
#include "../includes/ResourceManager.h"
#include "../includes/Model.h"
#include "../includes/Engine.h"
#include "../includes/PointLight.h"

BasicScene::BasicScene()
{

	ResourceManager::loadShader( "shaders/basic.vs", "shaders/basic.fs", NULL, "basic_shader" );
	sceneObjects["testModel"] = new Model( "C:/Users/jason/Documents/Visual Studio 2017/Projects/OpenGL resources/nanosuit_reflection/nanosuit.obj" );
	sceneObjects["light1"] = new PointLight();
	camera = Camera( glm::vec3( 0.0f, 5.0f, 10.0f ) );

	glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );

}
void BasicScene::update( GLfloat delta )
{
	this->delta = delta;
	//std::cout << "I'm updating logic!" << std::endl;

	keyInput();

	Shader shader = ResourceManager::getShader( "basic_shader" ).use();

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective( glm::radians(camera.FOV), (float)Engine::instance().SCREEN_WIDTH / (float)Engine::instance().SCREEN_HEIGHT, 0.1f, 1000.0f );

	shader.setMatrix4( "view", view);
	shader.setMatrix4( "projection", projection );

	sceneObjects["testModel"]->setPos( glm::vec3( 0.0f, 1.0f * sin( glfwGetTime() ), 0.0f ));

}

void BasicScene::render()
{
	glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//std::cout << "I'm rendering to a buffer!" << std::endl;
	Shader shader = ResourceManager::getShader( "basic_shader" );
	for (auto obj : sceneObjects)
	{
		glm::mat4 model = obj.second->updateModelMatrix();
		shader.setMatrix4( "model", model );
		obj.second->draw( shader );
	}
}

void BasicScene::keyInput()
{
	// Pressed keys are stored in the scene's keys array
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard( FORWARD, delta );
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard( BACKWARD, delta );
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard( LEFT, delta );
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard( RIGHT, delta );
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard( UP, delta );
	if (keys[GLFW_KEY_LEFT_CONTROL])
		camera.ProcessKeyboard( DOWN, delta );
	if (keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard( SPEEDUP, delta );
	if (keys[GLFW_KEY_LEFT_ALT])
		camera.ProcessKeyboard( SLOWDOWN, delta );
}
GLfloat lastX = Engine::instance().SCREEN_WIDTH / 2.0f, lastY = Engine::instance().SCREEN_HEIGHT / 2.0f;
GLboolean firstMouse = true;
void BasicScene::mouseInput( double xpos, double ypos )
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement( xoffset, yoffset );
}

void BasicScene::scrollInput( double xpos, double ypos )
{
	camera.ProcessMouseScroll( ypos );
}

void BasicScene::exit()
{
	std::cout << "I'm exiting!" << std::endl;
}

#include "../includes/Scene.h"
#include <iostream>
#include "../includes/BasicScene.h"
#include "../includes/ResourceManager.h"
#include "../includes/Model.h"
#include "../includes/Engine.h"
#include "../includes/PointLight.h"

glm::mat4 prevProj;
glm::mat4 prevView;

BasicScene::BasicScene()
{
	ResourceManager::loadShader( "shaders/basic.vs", "shaders/basic.fs", NULL, "basic_shader" );
	sceneObjects["testModel"] = new Model( "assets/cube.obj" );
	//sceneObjects["light1"] = new PointLight();
	camera = Camera( glm::vec3( 0.0f, 0.0f, 10.0f ) );

	glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );

	ResourceManager::createFramebuffer( "screen", Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT );
	ResourceManager::addColourTextureToFramebuffer( "screen", "colour1", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE );
	ResourceManager::loadShader( "shaders/quad.vs", "shaders/quad.fs", NULL, "quadShader" );

	prevProj = glm::mat4();
	prevView = glm::mat4();
}


void BasicScene::update( GLfloat delta )
{
	this->delta = delta;
	//std::cout << "I'm updating logic!" << std::endl;

	keyInput();

	Shader shader = ResourceManager::getShader( "basic_shader" ).use();

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective( glm::radians(camera.FOV), (float)Engine::instance().SCREEN_WIDTH / (float)Engine::instance().SCREEN_HEIGHT, 0.1f, 100.0f );
	shader.setMatrix4( "view", view);
	shader.setMatrix4( "projection", projection );

	shader = ResourceManager::getShader( "quadShader" ).use();
	shader.setMatrix4( "projection", projection );
	shader.setMatrix4( "view", view );

	shader.setMatrix4( "prevProjection", prevProj );
	shader.setMatrix4( "prevView", prevView );


	prevProj = projection;
	prevView = view;

	sceneObjects["testModel"]->setPos( glm::vec3( 0.0f, 1.0f * sin( glfwGetTime() ), 0.0f ));
	sceneObjects["testModel"]->setRot( 50.0f * glfwGetTime(), glm::vec3( 1.0f, 1.0f, 1.0f ));
}

void BasicScene::render()
{
	glEnable( GL_DEPTH_TEST );
	glBindFramebuffer( GL_FRAMEBUFFER, ResourceManager::getFramebuffer("screen").fbo);
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//std::cout << "I'm rendering to a buffer!" << std::endl;
	Shader shader = ResourceManager::getShader( "basic_shader" ).use();
	for (auto obj : sceneObjects)
	{
		glm::mat4 model = obj.second->updateModelMatrix();
		shader.setMatrix4( "model", model );
		obj.second->draw( shader );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClearColor( 1.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glDisable( GL_DEPTH_TEST );
	shader = ResourceManager::getShader( "quadShader" ).use();
	Framebuffer framebuffer = ResourceManager::getFramebuffer( "screen" );
	glActiveTexture( GL_TEXTURE0 );
	shader.setInteger( "screenTexture", 0 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.getColourTexture("colour1"));
	glActiveTexture( GL_TEXTURE1 );
	shader.setInteger( "depthTexture", 1 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.depthTexture );
	screenQuad.draw( shader );
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

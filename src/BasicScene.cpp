#include "../includes/Scene.h"
#include <iostream>
#include "../includes/BasicScene.h"
#include "../includes/ResourceManager.h"
#include "../includes/Model.h"
#include "../includes/Engine.h"
#include "../includes/PointLight.h"
#include <glm/gtx/string_cast.hpp>

glm::mat4 prevProj;
glm::mat4 prevView;

BasicScene::BasicScene()
{
	ResourceManager::loadShader( "shaders/basic.vs", "shaders/basic.fs", NULL, "basic_shader" );
	ResourceManager::loadShader( "shaders/quad.vs", "shaders/quad.fs", NULL, "post_process" );
	ResourceManager::loadShader( "shaders/light.vs", "shaders/light.fs", NULL, "light_shader" );
	ResourceManager::createFramebuffer( "screen", Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT );
	ResourceManager::addColourTextureToFramebuffer( "screen", "colour1", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE );
	glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	prevProj = glm::mat4();
	prevView = glm::mat4();

	camera = Camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );

	Material cubeMaterial;
	cubeMaterial.ambient = glm::vec3( 0.5f, 0.0f, 0.0f );
	cubeMaterial.diffuse = glm::vec3( 0.5f, 0.0f, 0.0f );
	cubeMaterial.specular = glm::vec3( 0.5f, 0.5f, 0.5f );
	cubeMaterial.shininess = 32.0f;
	Material planeMaterial;
	planeMaterial.ambient = glm::vec3( 0.0f, 0.5f, 0.0f );
	planeMaterial.diffuse = glm::vec3( 0.0f, 0.5f, 0.0f );
	planeMaterial.specular = glm::vec3( 0.5f, 0.5f, 0.5f );
	planeMaterial.shininess = 32.0f;

	sceneObjects["cube"] = new Cube();
	sceneObjects["cube"]->setMaterial( cubeMaterial );
	sceneObjects["plane"] = new Plane();
	sceneObjects["plane"]->setMaterial( planeMaterial );

	sceneLights["light1"] = new PointLight( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 2.0f, 2.0f, 2.0f ), true);

}


void BasicScene::update( GLfloat delta )
{
	keyInput( delta );
	updateShaderUniforms( delta );

	sceneObjects["cube"]->setPos( glm::vec3( 0.0f, 1.0f * sin( glfwGetTime() ), 0.0f ) );
	sceneObjects["cube"]->setRot( 50.0f * glfwGetTime(), glm::vec3( 1.0f, 1.0f, 0.5f ) );
	sceneObjects["plane"]->setPos( glm::vec3( 0.0f, -2.0f, 0.0f ) );
	sceneObjects["plane"]->setScale( glm::vec3( 50.0f, 0.0f, 50.0f ) );
}

void BasicScene::updateShaderUniforms( GLfloat delta )
{
	// Matrices
	Shader shader = ResourceManager::getShader( "basic_shader" ).use();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective( glm::radians( camera.FOV ), (float)Engine::instance().SCREEN_WIDTH / (float)Engine::instance().SCREEN_HEIGHT, 0.1f, 100.0f );
	shader.setVP( view, projection );
	shader.setVector3f( "cameraPos", camera.Position );
	sceneLights["light1"]->setUniforms( shader );
	
	shader = ResourceManager::getShader( "post_process" ).use();
	shader.setMatrix4( "inverseViewProjection", glm::inverse( projection * view ) );
	shader.setMatrix4( "prevViewProjection", (prevProj * prevView) );
	prevProj = projection;
	prevView = view;


	// Lights
	shader = ResourceManager::getShader( "light_shader" ).use();
	shader.setVP( view, projection );

	// Note to self: I used this to learn about how the coordinates are transformed since I was struggling to understand the important of the divide by W in the shader
	// as I was getting pretty much the same result without it. But with this I have learned that I don't get the correct world position without it.
	// The reason it works is that it gives a similar velocity vector anyway so for a full screen motion blur this doesn't matter. But if I was using deferrered rendering
	// and wanted to properly construct the world position then I need to divide by W to correct the perspective divide.
	// My assumption is that since the projection is being inversed then you can reverse the divide by dividing by the inverse. That's just maths.
	// This seems to agree with my assumption! http://www.iquilezles.org/blog/?p=1911
	/*glBindFramebuffer( GL_FRAMEBUFFER, ResourceManager::getFramebuffer( "screen" ).fbo );
	glm::vec2 texCoords = glm::vec2( 0.5f, 0.5f );
	GLfloat depth;
	glReadPixels( 400, 300, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth );
	std::cout << "Depth: " << depth << std::endl << std::endl;
	glm::vec4 NDC = glm::vec4( texCoords.x, texCoords.y, depth, 1 );
	std::cout << "NDC Position: " << glm::to_string( NDC ) << std::endl << std::endl;
	glm::vec4 clipSpace = glm::vec4( NDC.x * 2.0f - 1.0f, NDC.y * 2.0f - 1.0f, depth * 2.0f - 1.0f, 1.0f );
	std::cout << "Clip space position: " << glm::to_string( clipSpace ) << std::endl << std::endl;;
	glm::vec4 worldPos = glm::inverse( projection * view ) * clipSpace;
	std::cout << "WorldPos position: " << glm::to_string( worldPos ) << std::endl << std::endl;
	worldPos /= worldPos.w;
	std::cout << "WorldPos divided by W: " << glm::to_string( worldPos ) << std::endl;
	std::cout << "---------------------------------------------------------" << std::endl;
	glBindFramebuffer( GL_FRAMEBUFFER, 0);*/
}

void BasicScene::render()
{
	glEnable( GL_DEPTH_TEST );
	glBindFramebuffer( GL_FRAMEBUFFER, ResourceManager::getFramebuffer("screen").fbo);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.8f, 0.8f, 0.8f, 1.0f );
	//std::cout << "I'm rendering to a buffer!" << std::endl;
	Shader shader = ResourceManager::getShader( "basic_shader" ).use();
	for (auto obj : sceneObjects)
	{
		glm::mat4 model = obj.second->updateModelMatrix();
		shader.setMatrix4( "model", model );
		obj.second->getMaterial().setUniforms(shader);
		obj.second->draw( shader );
	}

	shader = ResourceManager::getShader( "light_shader" ).use();
	for (auto obj : sceneLights)
	{	
		if (obj.second->visible)
		{
			glm::mat4 model = obj.second->updateModelMatrix();
			shader.setMatrix4( "model", model );
			obj.second->draw( shader );
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glDisable( GL_DEPTH_TEST );
	shader = ResourceManager::getShader( "post_process" ).use();
	Framebuffer framebuffer = ResourceManager::getFramebuffer( "screen" );
	glActiveTexture( GL_TEXTURE0 );
	shader.setInteger( "screenTexture", 0 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.getColourTexture("colour1"));
	glActiveTexture( GL_TEXTURE1 );
	shader.setInteger( "depthTexture", 1 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.depthTexture );
	screenQuad.draw( shader );
}

void BasicScene::keyInput(GLfloat delta)
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

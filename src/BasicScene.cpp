#pragma once
#include <iostream>
#include "../includes/BasicScene.h"
#include "../includes/Model.h"
#include "../includes/Input.h"
#include <glm/gtx/string_cast.hpp>
#include "../includes/ResourceManager.h"
#include "../includes/PointLight.h"
glm::mat4 prevProj;
glm::mat4 prevView;
GLboolean alternateView = false;
GLboolean mouseLocked = true;
GLuint alternateScreenTexture;
GLint shadowWidth = 1024, shadowHeight = 1024;

BasicScene::BasicScene()
{
	ResourceManager::loadShader( "shaders/basic.vs", "shaders/basic.fs", NULL, "basic_shader" );
	ResourceManager::loadShader( "shaders/postprocess/quad.vs", "shaders/postprocess/quad.fs", NULL, "post_process" );
	ResourceManager::loadShader( "shaders/light.vs", "shaders/light.fs", NULL, "light_shader" );
	ResourceManager::loadShader( "shaders/shadowdepth.vs", "shaders/shadowdepth.fs", NULL, "shadowdepth" );
	ResourceManager::createFramebuffer( "post_process_screen", Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT );
	ResourceManager::addColourTextureToFramebuffer( "post_process_screen", "colour1", GL_RGB, GL_RGB, GL_UNSIGNED_BYTE );
	ResourceManager::createFramebuffer( "light_shadow_buffer", shadowWidth, shadowHeight, true, true);
	glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	prevProj = glm::mat4();
	prevView = glm::mat4();

	camera = Camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );

	Material teapotMaterial;
	teapotMaterial.ambient = glm::vec3( 0.8f, 0.0f, 0.0f );
	teapotMaterial.diffuse = glm::vec3( 0.8f, 0.0f, 0.0f );
	teapotMaterial.specular = glm::vec3( 1.0f, 1.0f, 1.0f );
	teapotMaterial.shininess = 10.0f;
	Material planeMaterial;
	planeMaterial.ambient = glm::vec3( 0.0f, 0.5f, 0.5f );
	planeMaterial.diffuse = glm::vec3( 0.0f, 0.5f, 0.5f );
	planeMaterial.specular = glm::vec3( 0.5f, 0.5f, 0.5f );
	planeMaterial.shininess = 32.0f;

	sceneObjects["teapot"] = new Model("assets/teapot/teapot.obj");
	sceneObjects["teapot"]->setMaterial( teapotMaterial );
	sceneObjects["plane"] = new Plane();
	sceneObjects["plane"]->setMaterial( planeMaterial );

	sceneLights["light1"] = new PointLight( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 2.0f, 3.0f, 2.0f ), true);

}


void BasicScene::update( GLfloat delta )
{

	updateShaderUniforms( delta );

	sceneObjects["teapot"]->setPos( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	//sceneObjects["teapot"]->setRot( 50.0f * glfwGetTime(), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	sceneObjects["teapot"]->setScale( glm::vec3( 0.03f, 0.03f, 0.03f ));
	sceneObjects["plane"]->setPos( glm::vec3( 0.0f, -2.0f, 0.0f ) );
	sceneObjects["plane"]->setScale( glm::vec3( 50.0f, 50.0f, 50.0f ) );
	sceneLights["light1"]->setPos( glm::vec3( 5.0f, 1.5 + 3.0f * sin(glfwGetTime()), 5.0f ));
}

void BasicScene::updateShaderUniforms( GLfloat delta )
{
	// Basic shader
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


	// Shadow shader
	glm::mat4 lightProjection = glm::perspective( glm::radians( 40.0f ), 1.0f, 0.1f, 100.0f );
	glm::mat4 lightView = glm::lookAt( sceneLights["light1"]->getPos(), glm::vec3( 0.0f, 2.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	shader = ResourceManager::getShader( "shadowdepth" ).use();
	shader.setMatrix4( "lightSpaceMatrix", lightSpaceMatrix );

	shader = ResourceManager::getShader( "basic_shader" ).use();
	shader.setMatrix4( "lightSpaceMatrix", lightSpaceMatrix );

	
	// Lights
	shader = ResourceManager::getShader( "light_shader" ).use();
	shader.setVP( view, projection );


	// Note to self: I used this to learn about how the coordinates are transformed since I was struggling to understand the important of the divide by W in the shader
	// as I was getting pretty much the same result without it. But with this I have learned that I don't get the correct world position without it.
	// The reason it works is that it gives a similar velocity vector anyway so for a full screen motion blur this doesn't matter. But if I was using deferrered rendering
	// and wanted to properly construct the world position then I need to divide by W to correct the perspective divide.
	// My assumption is that since the projection is being inversed then you can reverse the divide by dividing by the inverse. That's just maths.
	// This seems to agree with my assumption! http://www.iquilezles.org/blog/?p=1911
	/*glBindFramebuffer( GL_FRAMEBUFFER, ResourceManager::getFramebuffer( "post_process_screen" ).fbo );
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

	// Render to depth map for shadows
	glViewport( 0, 0, shadowWidth, shadowHeight );
	ResourceManager::getFramebuffer( "light_shadow_buffer" ).bind();
	glClear( GL_DEPTH_BUFFER_BIT );
	Shader shader = ResourceManager::getShader( "shadowdepth" ).use();
	for (auto obj : sceneObjects)
	{
		glm::mat4 model = obj.second->getModelMatrix();
		shader.setMatrix4( "model", model );
		//obj.second->getMaterial().setUniforms( shader );
		obj.second->draw( shader );
	}


	// Normal rendering pass
	glViewport( 0, 0, Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT );
	ResourceManager::getFramebuffer( "post_process_screen" ).bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.8f, 0.8f, 0.8f, 1.0f );
	//std::cout << "I'm rendering to a buffer!" << std::endl;
	shader = ResourceManager::getShader( "basic_shader" ).use();
	glActiveTexture( GL_TEXTURE4 );
	glBindTexture( GL_TEXTURE_2D, ResourceManager::getFramebuffer( "light_shadow_buffer" ).depthTexture );
	shader.setInteger( "depthMap", 4 );
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
	Framebuffer framebuffer = ResourceManager::getFramebuffer( "post_process_screen" );
	glActiveTexture( GL_TEXTURE0 );
	shader.setInteger( "screenTexture", 0 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.getColourTexture("colour1") );
	glActiveTexture( GL_TEXTURE1 );
	shader.setInteger( "depthTexture", 1 );
	glBindTexture( GL_TEXTURE_2D, framebuffer.depthTexture );
	screenQuad.draw( shader );
}
// Updated each frame
void BasicScene::keyInput(GLfloat delta)
{
	camera.ProcessKeyboard( delta );
	if (Input::keyWasPressed( GLFW_KEY_LEFT_ALT ))
	{
		if (mouseLocked)
		{
			glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
			mouseLocked = false;
		}
		else
		{
			glfwSetInputMode( Engine::instance().getCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
			camera.resetMouse();
			mouseLocked = true;
		}
	}
}
// Each frame
void BasicScene::mouseButtonInput()
{
	if (Input::mouseButtonWasPressed( GLFW_MOUSE_BUTTON_1 ))
	{
		//std::pair<GLfloat, GLfloat> mousePos = Input::getMousePos();
		//std::cout << "Mouse button 1 pressed at: " << mousePos.first << ", " << mousePos.second << std::endl;
	}
}
// Updated only when mouse is moved
void BasicScene::mouseInput()
{
	if(mouseLocked)
		camera.ProcessMouseMovement();
}
// Updated only when scrolled
void BasicScene::scrollInput()
{
	camera.ProcessMouseScroll();
}

BasicScene::~BasicScene()
{
	std::cout << "I'm exiting!" << std::endl;
}


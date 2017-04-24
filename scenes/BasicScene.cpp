#pragma once
#include <iostream>
#include "../scenes/BasicScene.h"
#include "../includes/Model/Model.h"
#include "../includes/Input/Input.h"
#include <glm/gtx/string_cast.hpp>
#include "../includes/PointLight/PointLight.h"
#include "../includes/FBO/FBO.h"
#include "../includes/ResourceManager/ResourceManager.h"
#include "../includes/Engine/Engine.h"

#include <stdlib.h> 

glm::mat4 prevProj;
glm::mat4 prevView;
GLboolean alternateView = false;
GLboolean mouseLocked = true;
GLuint alternateScreenTexture;
GLint shadowWidth = 2048, shadowHeight = 2048;

BasicScene::BasicScene()
{
	// Shaders
	shaders["basic_shader"] = std::unique_ptr<Shader>(new Shader("shaders/basic.vs", "shaders/basic.fs", NULL));
	shaders["post_process"] = std::unique_ptr<Shader>( new Shader( "shaders/postprocess/quad.vs", "shaders/postprocess/quad.fs", NULL));
	shaders["light_shader"] = std::unique_ptr<Shader>( new Shader( "shaders/light.vs", "shaders/light.fs", NULL));
	shaders["shadow_depth"] = std::unique_ptr<Shader>( new Shader( "shaders/shadowdepth.vs", "shaders/shadowdepth.fs", NULL));

	// Framebuffers
	framebuffers["post_process_screen"] = std::unique_ptr<FBO>( new FBO() );
	framebuffers["post_process_screen"]->createColour("colour1", GL_TEXTURE_2D, 0, GL_RGB, Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	framebuffers["post_process_screen"]->createDepth( GL_TEXTURE_2D, 0, Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT, GL_FLOAT );

	//ResourceManager::createFramebuffer( "light_shadow_buffer", shadowWidth, shadowHeight, true, true);
	framebuffers["light_shadow_buffer"] = std::unique_ptr<FBO>( new FBO() );
	framebuffers["light_shadow_buffer"]->createDepth( GL_TEXTURE_2D, 0, shadowWidth, shadowHeight, GL_FLOAT);
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

	sceneObjects["teapot"] = std::unique_ptr<Model>(new Model("assets/teapot/teapot.obj"));
	//sceneObjects["teapot"]->setMaterial( teapotMaterial );
	sceneObjects["plane"] = std::unique_ptr<Plane>(new Plane());
	sceneObjects["plane"]->setMaterial( planeMaterial );

	pointLights["light1"] = std::unique_ptr<PointLight>(new PointLight( glm::vec3( 1.0f, 1.0f, 1.0f ), glm::vec3( 5.0f, 3.0f, 5.0f ), true));

}


void BasicScene::update( GLfloat delta )
{

	updateShaderUniforms( delta );

	sceneObjects["teapot"]->setPos(0.0f, 2.0f, 0.0f );
	//sceneObjects["teapot"]->setRot( 50.0f * glfwGetTime(), 0.0f, 1.0f, 0.0f );
	sceneObjects["teapot"]->setScale(0.05f, 0.05f, 0.05f);
	sceneObjects["plane"]->setPos( 0.0f, 0.0f, 0.0f );
	sceneObjects["plane"]->setScale( 50.0f, 50.0f, 50.0f);
	pointLights["light1"]->setPos( 15.0f * sin(glfwGetTime()), 15.0f, 15.0f * cos(glfwGetTime()));
}

void BasicScene::updateShaderUniforms( GLfloat delta )
{
	// Basic shader
	Shader &basic_shader = shaders["basic_shader"]->use();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective( glm::radians( camera.FOV ), (float)Engine::instance().SCREEN_WIDTH / (float)Engine::instance().SCREEN_HEIGHT, 0.1f, 10000.0f );
	basic_shader.setVP( view, projection );
	basic_shader.setVector3f( "cameraPos", camera.Position );
	pointLights["light1"]->setUniforms( basic_shader );

	
	Shader &post_process_shader = shaders["post_process"]->use();
	post_process_shader.setMatrix4( "inverseViewProjection", glm::inverse( projection * view ) );
	post_process_shader.setMatrix4( "prevViewProjection", (prevProj * prevView) );
	prevProj = projection;
	prevView = view;


	// Shadow shader
	glm::mat4 lightProjection = glm::perspective( glm::radians( 40.0f ), 1.0f, 0.1f, 100.0f );
	glm::mat4 lightView = glm::lookAt( pointLights["light1"]->getPos(), glm::vec3( 0.0f, 2.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	Shader &shadow_depth = shaders["shadow_depth"]->use();
	shadow_depth.setMatrix4( "lightSpaceMatrix", lightSpaceMatrix );

	// Use basic shader again so I can set the lightspacematrix
	basic_shader.use();
	basic_shader.setMatrix4( "lightSpaceMatrix", lightSpaceMatrix );

	
	// Lights
	Shader &light_shader = shaders["light_shader"]->use();
	light_shader.setVP( view, projection );


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
	framebuffers["light_shadow_buffer"]->bind();
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glClear( GL_DEPTH_BUFFER_BIT );
	Shader &shadow_depth = shaders["shadow_depth"]->use();
	for (auto &obj : sceneObjects)
	{
		glm::mat4 model = obj.second->getModelMatrix();
		shadow_depth.setMatrix4( "model", model );
		//sobj.second->getMaterial().setUniforms( shader );
		obj.second->draw( shadow_depth );
	}
	glDisable( GL_CULL_FACE );

	// Normal rendering pass
	glViewport( 0, 0, Engine::instance().SCREEN_WIDTH, Engine::instance().SCREEN_HEIGHT );
	framebuffers["post_process_screen"]->bind();
	//ResourceManager::getFramebuffer( "post_process_screen" ).bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
	//std::cout << "I'm rendering to a buffer!" << std::endl;
	Shader &basic_shader = shaders["basic_shader"]->use();
	glActiveTexture( GL_TEXTURE10 );
	glBindTexture( GL_TEXTURE_2D, framebuffers["light_shadow_buffer"]->depthTexture );
	basic_shader.setInteger( "depthMap", 10 );
	for (auto &obj : sceneObjects)
	{
		glm::mat4 model = obj.second->updateModelMatrix();
		basic_shader.setMatrix4( "model", model );
		obj.second->draw( basic_shader );
	}

	Shader &light_shader = shaders["light_shader"]->use();
	for (auto &obj : pointLights)
	{	
		if (true)
		{
			glm::mat4 model = obj.second->updateModelMatrix();
			light_shader.setMatrix4( "model", model );
			obj.second->draw( light_shader );
		}
	}

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glDisable( GL_DEPTH_TEST );
	Shader &post_process = shaders["post_process"]->use();
	FBO* framebuffer = framebuffers["post_process_screen"].get();
	glActiveTexture( GL_TEXTURE0 );
	post_process.setInteger( "screenTexture", 0 );
	glBindTexture( GL_TEXTURE_2D, framebuffer->colourTextures["colour1"] );
	glActiveTexture( GL_TEXTURE1 );
	post_process.setInteger( "depthTexture", 1 );
	glBindTexture( GL_TEXTURE_2D, framebuffer->depthTexture );
	screenQuad.draw( post_process );
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

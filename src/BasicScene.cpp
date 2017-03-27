#include "../includes/Scene.h"
#include <iostream>
#include "../includes/BasicScene.h"
#include "../includes/ResourceManager.h"

BasicScene::BasicScene()
{
	GLfloat vertices[] = {
		0.0f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	glGenVertexArrays( 1, &triangleVAO );
	glGenBuffers( 1, &triangleVBO );
	glBindVertexArray( triangleVAO );
	glBindBuffer( GL_ARRAY_BUFFER, triangleVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (GLvoid*)0 );
	glEnableVertexAttribArray( 0 );

	ResourceManager::loadShader( "shaders/basic.vs", "shaders/basic.fs", NULL, "basic_shader" );
}
void BasicScene::update( GLfloat delta )
{
	std::cout << "I'm updating logic!" << std::endl;
}

void BasicScene::render()
{
	std::cout << "I'm rendering to a buffer!" << std::endl;
	ResourceManager::getShader( "basic_shader" ).use();
	glBindVertexArray( triangleVAO );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glBindVertexArray( 0 );
}

void BasicScene::exit()
{
	std::cout << "I'm exiting!" << std::endl;
}

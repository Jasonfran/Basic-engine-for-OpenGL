#pragma once
#include <GL\glew.h>
#include "Shader.h"
#include "SceneObject.h"
#include <iostream>
#include "Material.h"
class TexturedScreenQuad
{
public:
	TexturedScreenQuad()
	{
		if (!VAO)
		{
			glGenVertexArrays( 1, &VAO );
			glGenBuffers( 1, &VBO );
			glBindVertexArray( VAO );
			glBindBuffer( GL_ARRAY_BUFFER, VBO );
			glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)0 );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
			glBindVertexArray( 0 );
		}
	}

	void draw( Shader shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		glBindVertexArray( 0 );
	}
private:
	static GLfloat vertices[20];
	static GLuint VAO, VBO;
};

class Cube : public SceneObject
{
public:
	Cube()
	{
		if (!VAO)
		{
			glGenVertexArrays( 1, &VAO );
			glGenBuffers( 1, &VBO );
			glBindVertexArray( VAO );
			glBindBuffer( GL_ARRAY_BUFFER, VBO );
			glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)0 );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
			glEnableVertexAttribArray( 2 );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)(6 * sizeof( GLfloat )) );
			glBindVertexArray( 0 );
		}
	}
	void draw( Shader shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
		glBindVertexArray( 0 );
	}

private:
	static GLfloat vertices[288];
	static GLuint VAO, VBO;
};

class Plane : public SceneObject{
public:
	Plane()
	{
		if (!VAO)
		{
			glGenVertexArrays( 1, &VAO );
			glGenBuffers( 1, &VBO );
			glBindVertexArray( VAO );
			glBindBuffer( GL_ARRAY_BUFFER, VBO );
			glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)0 );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
			glEnableVertexAttribArray( 2 );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), (GLvoid*)(6 * sizeof( GLfloat )) );
			glBindVertexArray( 0 );
		}
	}
	void draw( Shader shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );
	}
private:
	static GLfloat vertices[48];
	static GLuint VAO, VBO;
};
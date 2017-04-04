#pragma once
#include <GL\glew.h>
#include "Shader.h"

class TexturedScreenQuad
{
public:
	TexturedScreenQuad()
	{
		glGenVertexArrays( 1, &VAO );
		glGenBuffers( 1, &VBO );
		glBindVertexArray( VAO );
		glBindBuffer( GL_ARRAY_BUFFER, VBO );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)0 );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )));
		glBindVertexArray( 0 );
		std::cout << "Screen quad created!" << std::endl;
	}

	void draw( Shader shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		glBindVertexArray( 0 );
	}
private:
	GLfloat vertices[20] = {
		// Positions        // Texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	GLuint VAO, VBO;
};
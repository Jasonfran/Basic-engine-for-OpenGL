#pragma once
#include "SceneObject.h"

class PointLight : public SceneObject
{
public:
	GLboolean visible;
	PointLight( glm::vec3 colour, glm::vec3 position, GLboolean visible = false )
	{
		if (!VAO)
		{
			glGenVertexArrays( 1, &VAO );
			glGenBuffers( 1, &VBO );
			glBindVertexArray( VAO );
			glBindBuffer( GL_ARRAY_BUFFER, VBO );
			glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (GLvoid*)0 );
			glBindVertexArray( 0 );
		}

		this->colour = colour;
		this->setPos( position );
		this->visible = visible; 
		this->setScale(glm::vec3( 0.3f, 0.3f, 0.3f ));

	}
	void draw( Shader shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
		glBindVertexArray( 0 );
	}
	
	void setUniforms( Shader shader, GLboolean useShader = false )
	{
		if (useShader)
			shader.use();
		shader.setVector3f( "light.colour", colour );
		shader.setVector3f( "light.position", getPos() );
		shader.setFloat( "light.attenution", attenuation );
	}

	void setAttenuation( GLfloat att )
	{
		attenuation = att;
	}

	glm::vec3 getColour()
	{
		return colour;
	}
	~PointLight()
	{
		std::cout << "Pointlight gone" << std::endl;
	}
private:
	static GLfloat vertices[180];
	static GLuint VAO, VBO;
	glm::vec3 colour = glm::vec3( 1.0f, 1.0f, 1.0f );
	GLfloat attenuation = 0.0f;
};
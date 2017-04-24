#pragma once
#include "../SceneObject/SceneObject.h"

class PointLight : public SceneObject
{
public:

	PointLight( glm::vec3 colour, glm::vec3 position, GLboolean visible = false )
	{
		this->setupLight( colour, position, visible );
	}

	PointLight()
	{
		this->setupLight( glm::vec3(1.0f), glm::vec3(0.0f), true );
	}

	void draw( Shader &shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
		glBindVertexArray( 0 );
	}
	
	void setUniforms( Shader &shader, GLboolean useShader = false )
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

	void setupLight( glm::vec3 colour, glm::vec3 position, GLboolean visible )
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
		this->setPos( position.x, position.y, position.z );
		this->setVisibility(visible);
		this->setScale( 0.3f, 0.3f, 0.3f );
	}

	static GLfloat vertices[180];
	static GLuint VAO, VBO;
	glm::vec3 colour = glm::vec3( 1.0f, 1.0f, 1.0f );
	GLfloat attenuation = 0.0f;
};
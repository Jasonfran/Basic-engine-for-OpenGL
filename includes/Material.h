#pragma once
#include <GL/glew.h>
#include <glm/common.hpp>
#include "Shader/Shader.h"

struct Material
{
public:
	glm::vec3 ambient;
	GLuint ambientTexture;
	GLint usesAmbientTexture = 0;
	glm::vec3 diffuse;
	GLuint diffuseTexture;
	GLint usesDiffuseTexture = 0;
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	GLuint specularTexture;
	GLint usesSpecularTexture = 0;
	GLuint normalTexture;
	GLint usesNormalTexture = 0;
	GLfloat shininess = 16.0f;
	GLfloat opacity = 1.0f;
	
	void setUniforms( Shader &shader, GLboolean useShader = false )
	{
		if (useShader)
			shader.use();
		if (usesAmbientTexture)
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, ambientTexture );
			shader.setInteger( "material.ambientTexture", 0 );
		} else
			shader.setVector3f( "material.ambient", ambient, false );

		if (usesDiffuseTexture)
		{
			glActiveTexture( GL_TEXTURE1 );
			glBindTexture( GL_TEXTURE_2D, diffuseTexture );
			shader.setInteger( "material.diffuseTexture", 1 );
		}
		else
			shader.setVector3f( "material.diffuse", diffuse, false );
		if (usesSpecularTexture)
		{
			glActiveTexture( GL_TEXTURE2 );
			glBindTexture( GL_TEXTURE_2D, specularTexture );
			shader.setInteger( "material.specularTexture", 2 );
		}
		else
			shader.setVector3f( "material.specular", specular, false );

		if (usesNormalTexture)
		{
			glActiveTexture( GL_TEXTURE3 );
			glBindTexture( GL_TEXTURE_2D, normalTexture );
			shader.setInteger( "material.normalTexture", 3 );
		}
		shader.setInteger( "material.usesAmbientTexture", usesAmbientTexture );
		shader.setInteger( "material.usesDiffuseTexture", usesDiffuseTexture );
		shader.setInteger( "material.usesSpecularTexture", usesSpecularTexture );
		shader.setInteger( "material.usesNormalTexture", usesNormalTexture );
		shader.setFloat( "material.shininess", shininess, false );
		shader.setFloat("material.opacity", opacity, false);
		glActiveTexture( GL_TEXTURE0 );
	}
};
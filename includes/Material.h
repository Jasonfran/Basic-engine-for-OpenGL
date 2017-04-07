#pragma once
#include <GL/glew.h>
#include <glm/common.hpp>
#include "Shader.h"

struct Material
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
	
	void setUniforms( Shader shader, GLboolean useShader = false )
	{
		if (useShader)
			shader.use();
		shader.setVector3f( "material.ambient", ambient, false );
		shader.setVector3f( "material.diffuse", diffuse, false );
		shader.setVector3f( "material.specular", specular, false );
		shader.setFloat( "material.shininess", shininess, false );
	}
};
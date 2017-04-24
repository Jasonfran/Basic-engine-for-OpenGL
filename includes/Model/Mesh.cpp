#include "Mesh.h"
#include "../Material.h"
#include <iostream>

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, std::vector<glm::vec3> colours )
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->colours = colours; //0 = ambient, 1 = diffuse, 2 = specular, first element of 3 = specular exponent

	this->setupMesh();
}

void Mesh::draw( Shader &shader )
{
	glBindVertexArray( VAO );
	glDrawElements( GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray( 0 );
}

void Mesh::setupMesh()
{
	glGenVertexArrays( 1, &VAO );
	glGenBuffers( 1, &VBO );
	glGenBuffers( 1, &EBO );

	glBindVertexArray( VAO );

	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( Vertex ),
		&vertices[0], GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
		&indices[0], GL_STATIC_DRAW);

	// Positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)0 );
	// Normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)offsetof( Vertex, normal ) );
	// Texture coordinates
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)offsetof( Vertex, texCoords ) );

	glBindVertexArray( 0 );

	GLuint numTextures = textures.size();
	Material material;
	//std::cout << numTextures << std::endl;
	for (GLuint i = 0; i < numTextures; i++)
	{
		MaterialType matType = textures[i].type;
		//std::cout << matType << std::endl;
		if (matType == AMBIENT)
		{
			material.ambientTexture = textures[i].id;
			material.usesAmbientTexture = 1;
		}
		else if (matType == DIFFUSE)
		{
			material.diffuseTexture = textures[i].id;
			material.usesDiffuseTexture = 1;
		}
		else if (matType == SPECULAR)
		{
			material.specularTexture = textures[i].id;
			material.usesSpecularTexture = 1;
		}
		else if (matType == NORMAL)
		{
			material.normalTexture = textures[i].id;
			material.usesNormalTexture = 1;
		}
	}

	if (!material.usesAmbientTexture && material.usesDiffuseTexture)
	{
		material.ambientTexture = material.diffuseTexture;
		material.usesAmbientTexture = 1;
	}

	if (!material.usesAmbientTexture && !material.usesDiffuseTexture)
	{
		material.ambient = colours[0];
		material.diffuse = colours[1];
	}

	if (material.ambient == glm::vec3( 0.0f ))
		material.ambient = material.diffuse;

	if (!material.usesDiffuseTexture)
	{
		material.diffuse = colours[1];
	}

	if (colours[3].b == 1.0f)
		material.specular = colours[2];

	if (colours[3].g == 1.0f)
		material.shininess = colours[3].r;

	this->setMaterial(material);
}

void Mesh::deleteBuffers()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );
}

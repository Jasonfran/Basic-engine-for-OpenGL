#include "Mesh.h"

Mesh::Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures )
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::draw( Shader shader )
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture( GL_TEXTURE0 + i );
		std::string number;
		std::string name;
		MaterialType matType = textures[i].type;
		if (matType == DIFFUSE)
		{
			name = "diffuse";
			number = std::to_string( diffuseNr++ );
		}
		else if (matType == SPECULAR)
		{
			name = "specular";
			number = std::to_string( specularNr++ );
		}

		shader.setInteger( ("material." + name + number).c_str(), i );
		glBindTexture( GL_TEXTURE_2D, textures[i].id );
	}
	glActiveTexture( GL_TEXTURE0 );

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

}

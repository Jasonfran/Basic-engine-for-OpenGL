#pragma once
#include <GL\glew.h>
#include "../Scene.h"
#include <iostream>
#include "../Material.h"
#include "../SceneObject/SceneObject.h"

class TexturedScreenQuad
{
public:
	TexturedScreenQuad()
	{
		if (!VAO || VAO == 0)
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
		numOfScreenQuads++;
	}

	void draw( Shader &shader )
	{
		shader.use();
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		glBindVertexArray( 0 );
	}

	~TexturedScreenQuad()
	{
		numOfScreenQuads--;
		if (numOfScreenQuads <= 0)
		{
			/*glDeleteVertexArrays( 1, &this->VAO );
			glDeleteBuffers( 1, &this->VBO );
			this->VAO = 0;*/
		}
	}
private:
	static int numOfScreenQuads;
	static GLfloat vertices[20];
	static GLuint VAO, VBO;
};

class Cube : public SceneObject
{
public:
	Cube()
	{
		if (!VAO || VAO == 0)
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
		numOfCubes++;
	}
	void draw( Shader &shader )
	{
		shader.use();
		this->getMaterial().setUniforms( shader );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 36 );
		glBindVertexArray( 0 );
	}

	~Cube()
	{
		numOfCubes--;
		if (numOfCubes <= 0)
		{
			/*glDeleteVertexArrays( 1, &this->VAO );
			glDeleteBuffers( 1, &this->VBO );
			this->VAO = 0;*/
		}
	}

private:
	static int numOfCubes;
	static GLfloat vertices[288];
	static GLuint VAO, VBO;
};

class Plane : public SceneObject{
public:
	Plane()
	{
		if (!VAO || VAO == 0)
		{
			// Calculating TBN matrix for ground plane.
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> uvCoords;
			// stride of array is 8. 3 elements for vertex pos, 3 for normal, 2 for uv. 6 vertices. 48 total elements
			for (int i = 0; i < 6; ++i)
			{
				positions.push_back( glm::vec3( this->vertices[0 + 8 * i], this->vertices[1 + 8 * i], this->vertices[2 + 8 * i] ) );
				normals.push_back( glm::vec3( this->vertices[3 + 8 * i], this->vertices[4 + 8 * i], this->vertices[5 + 8 * i] ) );
				uvCoords.push_back( glm::vec2( this->vertices[6 + 8 * i], this->vertices[7 + 8 * i] ) );
			}
			// Triangle 1 is 0-2, triangle 2 is 3-5
			// Need delta edge for 2 edges in both triangles
			// 1
			glm::vec3 tri1DeltaEdge1 = positions[1] - positions[0];
			glm::vec3 tri1DeltaEdge2 = positions[2] - positions[0];
			// 2
			glm::vec3 tri2DeltaEdge1 = positions[4] - positions[3];
			glm::vec3 tri2DeltaEdge2 = positions[5] - positions[3];

			// Now need delta from UV coords
			// 1
			glm::vec2 tri1DeltaUV1 = uvCoords[1] - uvCoords[0];
			glm::vec2 tri1DeltaUV2 = uvCoords[2] - uvCoords[0];
			// 2
			glm::vec2 tri2DeltaUV1 = uvCoords[4] - uvCoords[3];
			glm::vec2 tri2DeltaUV2 = uvCoords[5] - uvCoords[3];

			// 1
			GLfloat fract = 1.0f / (tri1DeltaUV1.x * tri1DeltaUV2.y - tri1DeltaUV2.x * tri1DeltaUV1.y);
			glm::vec3 tri1Tangent;
			tri1Tangent.x = fract * (tri1DeltaUV2.y * tri1DeltaEdge1.x - tri1DeltaUV1.y * tri1DeltaEdge2.x);
			tri1Tangent.y = fract * (tri1DeltaUV2.y * tri1DeltaEdge1.y - tri1DeltaUV1.y * tri1DeltaEdge2.y);
			tri1Tangent.z = fract * (tri1DeltaUV2.y * tri1DeltaEdge1.z - tri1DeltaUV1.y * tri1DeltaEdge2.z);
			tri1Tangent = glm::normalize( tri1Tangent );
			glm::vec3 tri1Bitangent;
			tri1Bitangent.x = fract * (-tri1DeltaUV2.x * tri1DeltaEdge1.x + tri1DeltaUV1.x * tri1DeltaEdge2.x);
			tri1Bitangent.y = fract * (-tri1DeltaUV2.x * tri1DeltaEdge1.y + tri1DeltaUV1.x * tri1DeltaEdge2.y);
			tri1Bitangent.z = fract * (-tri1DeltaUV2.x * tri1DeltaEdge1.z + tri1DeltaUV1.x * tri1DeltaEdge2.z);
			tri1Bitangent = glm::normalize( tri1Bitangent );
			// 2
			fract = 1.0f / (tri2DeltaUV1.x * tri2DeltaUV2.y - tri2DeltaUV2.x * tri2DeltaUV1.y);
			glm::vec3 tri2Tangent;
			tri2Tangent.x = fract * (tri2DeltaUV2.y * tri2DeltaEdge1.x - tri2DeltaUV1.y * tri2DeltaEdge2.x);
			tri2Tangent.y = fract * (tri2DeltaUV2.y * tri2DeltaEdge1.y - tri2DeltaUV1.y * tri2DeltaEdge2.y);
			tri2Tangent.z = fract * (tri2DeltaUV2.y * tri2DeltaEdge1.z - tri2DeltaUV1.y * tri2DeltaEdge2.z);
			tri2Tangent = glm::normalize( tri2Tangent );
			glm::vec3 tri2Bitangent;
			tri2Bitangent.x = fract * (-tri2DeltaUV2.x * tri2DeltaEdge1.x + tri2DeltaUV1.x * tri2DeltaEdge2.x);
			tri2Bitangent.y = fract * (-tri2DeltaUV2.x * tri2DeltaEdge1.y + tri2DeltaUV1.x * tri2DeltaEdge2.y);
			tri2Bitangent.z = fract * (-tri2DeltaUV2.x * tri2DeltaEdge1.z + tri2DeltaUV1.x * tri2DeltaEdge2.z);
			tri2Bitangent = glm::normalize( tri2Bitangent );

			this->tangent[0] = tri1Tangent;
			this->tangent[1] = tri1Tangent;
			this->tangent[2] = tri1Tangent;

			this->tangent[3] = tri2Tangent;
			this->tangent[4] = tri2Tangent;
			this->tangent[5] = tri2Tangent;

			this->bitangent[0] = tri1Bitangent;
			this->bitangent[1] = tri1Bitangent;
			this->bitangent[2] = tri1Bitangent;

			this->bitangent[3] = tri2Bitangent;
			this->bitangent[4] = tri2Bitangent;
			this->bitangent[5] = tri2Bitangent;

			// this is disgusting
			GLfloat bufferdata[] = {
				positions[0].x, positions[0].y, positions[0].z, normals[0].x, normals[0].y, normals[0].z, uvCoords[0].x, uvCoords[0].y, tangent[0].x, tangent[0].y, tangent[0].z, bitangent[0].x, bitangent[0].y, bitangent[0].z,
				positions[1].x, positions[1].y, positions[1].z, normals[1].x, normals[1].y, normals[1].z, uvCoords[1].x, uvCoords[1].y, tangent[1].x, tangent[1].y, tangent[1].z, bitangent[1].x, bitangent[1].y, bitangent[1].z,
				positions[2].x, positions[2].y, positions[2].z, normals[2].x, normals[2].y, normals[2].z, uvCoords[2].x, uvCoords[2].y, tangent[2].x, tangent[2].y, tangent[2].z, bitangent[2].x, bitangent[2].y, bitangent[2].z,
				positions[3].x, positions[3].y, positions[3].z, normals[3].x, normals[3].y, normals[3].z, uvCoords[3].x, uvCoords[3].y, tangent[3].x, tangent[3].y, tangent[3].z, bitangent[3].x, bitangent[3].y, bitangent[3].z,
				positions[4].x, positions[4].y, positions[4].z, normals[4].x, normals[4].y, normals[4].z, uvCoords[4].x, uvCoords[4].y, tangent[4].x, tangent[4].y, tangent[4].z, bitangent[4].x, bitangent[4].y, bitangent[4].z,
				positions[5].x, positions[5].y, positions[5].z, normals[5].x, normals[5].y, normals[5].z, uvCoords[5].x, uvCoords[5].y, tangent[5].x, tangent[5].y, tangent[5].z, bitangent[5].x, bitangent[5].y, bitangent[5].z,
			};

			glGenVertexArrays( 1, &VAO );
			glGenBuffers( 1, &VBO );
			glBindVertexArray( VAO );
			glBindBuffer( GL_ARRAY_BUFFER, VBO );
			glBufferData( GL_ARRAY_BUFFER, sizeof( bufferdata ), &bufferdata, GL_STATIC_DRAW );
			glEnableVertexAttribArray( 0 );
			glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof( GLfloat ), (GLvoid*)0 );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof( GLfloat ), (GLvoid*)(3 * sizeof( GLfloat )) );
			glEnableVertexAttribArray( 2 );
			glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof( GLfloat ), (GLvoid*)(6 * sizeof( GLfloat )) );
			glEnableVertexAttribArray( 3 );
			glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof( GLfloat ), (GLvoid*)(8 * sizeof( GLfloat )) );
			glEnableVertexAttribArray( 4 );
			glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof( GLfloat ), (GLvoid*)(11 * sizeof( GLfloat )) );
			glBindVertexArray( 0 );
		}
		numOfPlanes++;
	}
	void draw( Shader &shader )
	{
		shader.use();
		this->getMaterial().setUniforms( shader );
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 6 );
		glBindVertexArray( 0 );
	}

	~Plane()
	{
		numOfPlanes--;
		if (numOfPlanes <= 0)
		{
			/*glDeleteVertexArrays( 1, &this->VAO );
			glDeleteBuffers( 1, &this->VBO );
			this->VAO = 0;*/
		}
	}
	static GLfloat vertices[48];
	glm::vec3 tangent[6];
	glm::vec3 bitangent[6];
private:
	static int numOfPlanes;
	static GLuint VAO, VBO;
};
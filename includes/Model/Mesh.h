#pragma once

#include <glm\common.hpp>
#include <GL\glew.h>
#include <vector>
#include "../Shader/Shader.h"
#include "../assimp/types.h"
#include "../SceneObject/SceneObject.h"
enum MaterialType
{
	AMBIENT, DIFFUSE, SPECULAR, NORMAL
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	GLuint id;
	MaterialType type;
	aiString path;
};

class Mesh : public SceneObject
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<glm::vec3> colours; //0 = ambient, 1 = diffuse, 2 = specular, first element of 3 = specular exponent
	Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, std::vector<glm::vec3> colours );
	void draw( Shader &shader );
	void deleteBuffers();
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};
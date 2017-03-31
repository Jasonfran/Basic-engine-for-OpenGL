#pragma once

#include <glm\common.hpp>
#include <GL\glew.h>
#include <vector>
#include "../includes/Shader.h"

enum MaterialType
{
	DIFFUSE, SPECULAR, NORMAL
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
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures );
	void draw( Shader shader );
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};
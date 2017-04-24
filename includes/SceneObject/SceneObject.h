#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Material.h"
#include <iostream>
class Shader;

class SceneObject
{
public:

	virtual void setPos( GLfloat x, GLfloat y, GLfloat z );
	virtual void setRot( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
	virtual void setScale( GLfloat x, GLfloat y, GLfloat z );
	virtual glm::vec3 getPos() { return position; }
	virtual GLfloat getRotAngle() { return rotationAngle; };
	virtual glm::vec3 getRotAxis() { return rotationAxis; };
	virtual glm::vec3 getScale() { return scale; };
	glm::mat4 updateModelMatrix();
	glm::mat4 getModelMatrix() { return model; }

	virtual void setMaterial( Material mat ) { material = mat; }
	virtual Material getMaterial() { return material; }

	virtual GLboolean setVisibility( GLboolean visibility ) { visible = visibility; return visible; }
	virtual GLboolean isVisible() { return visible; }

	virtual void draw( Shader &shader ) = 0;

	virtual ~SceneObject() { std::cout << "Scene object destroyed" << std::endl; }
private:
	glm::mat4 model;
	glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f );
	GLfloat rotationAngle = glm::radians( 0.0f );
	glm::vec3 rotationAxis = glm::vec3( 0.0f, 1.0f, 0.0f );
	glm::vec3 scale = glm::vec3( 1.0f, 1.0f, 1.0f );
	Material material;
	GLboolean visible = true;
};
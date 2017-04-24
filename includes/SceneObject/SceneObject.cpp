#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

void SceneObject::setPos( GLfloat x, GLfloat y, GLfloat z )
{
	this->position = glm::vec3( x, y, z );
}

// Angle in degrees
void SceneObject::setRot( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	this->rotationAngle = glm::radians( angle );
	this->rotationAxis = glm::vec3( x, y, z );
}
//
void SceneObject::setScale( GLfloat x, GLfloat y, GLfloat z )
{
	this->scale = glm::vec3( x, y, z );
}
//
glm::mat4 SceneObject::updateModelMatrix()
{
	glm::mat4 tempmodel = glm::mat4();
	tempmodel = glm::translate( tempmodel, this->position );
	tempmodel = glm::rotate( tempmodel, this->rotationAngle, this->rotationAxis );
	tempmodel = glm::scale( tempmodel, this->scale );
	this->model = tempmodel;
	return this->model;
}



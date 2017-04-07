#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
class SceneObject
{
public:
	virtual void draw( Shader shader ) = 0;
	void setPos( glm::vec3 pos ) { position = pos; }
	glm::vec3 getPos() { return position; }
	void setRot( GLfloat angle, glm::vec3 axis ) { rotationAngle = glm::radians( angle ); rotationAxis = axis; }
	std::pair<GLfloat, glm::vec3> getRot() { return std::make_pair( rotationAngle, rotationAxis ); }
	void setScale( glm::vec3 scale ) { this->scale = scale; }
	glm::vec3 getScale() { return scale; }
	glm::mat4 updateModelMatrix()
	{
		glm::mat4 tempmodel = glm::mat4();
		tempmodel = glm::translate( tempmodel, position );
		tempmodel = glm::rotate( tempmodel, rotationAngle, rotationAxis );
		tempmodel = glm::scale( tempmodel, this->scale );
		model = tempmodel; //will store in case I want to stop updating model matrix but want the last matrix used
		return model;
	}
	glm::mat4 getModelMatrix() { return model; }
	void setMaterial( Material mat ) { material = mat; }
	Material getMaterial() { return material; }

private:
	glm::mat4 model;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat rotationAngle = glm::radians(0.0f);
	glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	Material material;
};
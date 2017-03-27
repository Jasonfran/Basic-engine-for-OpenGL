#pragma once
#include "../includes/Scene.h"
#include "../includes/Shader.h"

class BasicScene : public Scene
{
public:
	GLuint triangleVAO, triangleVBO;
	BasicScene();
	void update( GLfloat delta );
	void render();
	void exit();
};
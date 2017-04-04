#pragma once
#include "../includes/Scene.h"
#include "../includes/Shader.h"
#include "../includes/Model.h"
#include "../includes/Camera.h"
#include <GLFW/glfw3.h>
#include <memory>
#include "UsefulObjects.h"

class BasicScene : public Scene
{
public:
	BasicScene();
	void update( GLfloat delta);
	void render();
	void keyInput();
	void mouseInput( double xoffset, double yoffset );
	void scrollInput( double xpos, double ypos );
	void exit();
	TexturedScreenQuad screenQuad;
};
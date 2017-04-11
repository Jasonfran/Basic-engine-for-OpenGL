#pragma once
#include "Scene.h"
#include <GLFW/glfw3.h>
#include <memory>
#include "UsefulObjects.h"

class BasicScene : public Scene
{
public:
	BasicScene();
	void update( GLfloat delta);
	void updateShaderUniforms( GLfloat delta );
	void render();
	void keyInput(GLfloat delta);
	void mouseButtonInput();
	void mouseInput();
	void scrollInput();
	~BasicScene();
	TexturedScreenQuad screenQuad;
};
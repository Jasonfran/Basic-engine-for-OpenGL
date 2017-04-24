#pragma once
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <map>
class Scene;
class Input
{
public:
	static void updateKey( GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);
	static void updateMouseButton( GLFWwindow *window, GLint button, GLint action, GLint mods );
	static void updateStates();
	static bool keyWasPressed(GLint key);
	static bool keyIsHeld(GLint key);
	static bool mouseButtonWasPressed( GLint button );
	static bool mouseButtonIsHeld( GLint button );
	static void mouseInput( GLFWwindow * window, double xpos, double ypos );
	static std::pair<GLfloat, GLfloat> getMousePos() { return std::make_pair(mouseX, mouseY); }
	static void scrollInput( GLFWwindow * window, double xoffset, double yoffset );
	static std::pair<GLfloat, GLfloat> getScrollOffset() { return std::make_pair(scrollX, scrollY); }
private:
	static std::map<GLint, std::pair<GLboolean, GLboolean>> keyStates;
	static std::map<GLint, std::pair<GLboolean, GLboolean>> mouseButtonStates;
	static GLfloat mouseX;
	static GLfloat mouseY;
	static GLfloat scrollX;
	static GLfloat scrollY;
};
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	GLuint shaderID;
	Shader() {}
	Shader &use();
	void compile( const GLchar *vertSource, const GLchar *fragSource, const GLchar *geomSource = nullptr );
	void    setFloat( const GLchar *name, GLfloat value, GLboolean useShader = false );
	void    setInteger( const GLchar *name, GLint value, GLboolean useShader = false );
	void    setVector2f( const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false );
	void    setVector2f( const GLchar *name, const glm::vec2 &value, GLboolean useShader = false );
	void    setVector3f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false );
	void    setVector3f( const GLchar *name, const glm::vec3 &value, GLboolean useShader = false );
	void    setVector4f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false );
	void    setVector4f( const GLchar *name, const glm::vec4 &value, GLboolean useShader = false );
	void    setMatrix4( const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false );
private:
	void checkCompileErrors( GLuint object, std::string type );
};
#endif // !SHADER_H
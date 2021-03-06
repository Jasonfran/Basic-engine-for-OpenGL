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
	~Shader();
	Shader( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile );
	Shader &use();
	void reload();
	void compile( const GLchar *vertSource, const GLchar *fragSource, const GLchar *geomSource = nullptr );
	void setFloat( const GLchar *name, GLfloat value, GLboolean useShader = false );
	void setInteger( const GLchar *name, GLint value, GLboolean useShader = false );
	void setVector2f( const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false );
	void setVector2f( const GLchar *name, const glm::vec2 &value, GLboolean useShader = false );
	void setVector3f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false );
	void setVector3f( const GLchar *name, const glm::vec3 &value, GLboolean useShader = false );
	void setVector4f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false );
	void setVector4f( const GLchar *name, const glm::vec4 &value, GLboolean useShader = false );
	void setMatrix4( const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false );
	void setVP( glm::mat4 view, glm::mat4 projection, GLboolean useShader = false);
	const GLchar *vertPath, *fragPath, *geomPath;
private:
	void loadFromFile( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile );
	void checkCompileErrors( GLuint object, std::string type );
};
#endif // !SHADER_H
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader( const GLchar *vertFile, const GLchar *fragFile, const GLchar *geomFile)
{
	this->loadFromFile( vertFile, fragFile, geomFile);
}

Shader::~Shader()
{
	//glUseProgram( 0 );
	//glDeleteProgram( this->shaderID );
	std::cout << "Shader deleted" << std::endl;
}

Shader &Shader::use()
{
	glUseProgram( this->shaderID );
	return *this;
}

void Shader::reload()
{
	GLint currentShader;
	glGetIntegerv( GL_CURRENT_PROGRAM, &currentShader );
	if (currentShader == this->shaderID)
		glUseProgram( 0 );
	glDeleteProgram( this->shaderID );
	this->loadFromFile(this->vertPath, this->fragPath, this->geomPath);
}

void Shader::compile( const GLchar *vertSource, const GLchar *fragSource, const GLchar *geomSource )
{
	GLuint fragShader, vertShader, geomShader;
	vertShader = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertShader, 1, &vertSource, NULL );
	glCompileShader( vertShader );
	checkCompileErrors( vertShader, "VERTEX" );

	fragShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragShader, 1, &fragSource, NULL );
	glCompileShader( fragShader );
	checkCompileErrors( fragShader, "FRAGMENT" );

	if (geomSource != nullptr)
	{
		geomShader = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( geomShader, 1, &geomSource, NULL );
		glCompileShader( geomShader );
		checkCompileErrors( geomShader, "GEOMETRY" );
	}

	this->shaderID = glCreateProgram();
	glAttachShader( this->shaderID, vertShader );
	glAttachShader( this->shaderID, fragShader );
	if (geomSource != nullptr)
	{
		glAttachShader( this->shaderID, geomShader );
	}
	glLinkProgram( this->shaderID );
	checkCompileErrors( this->shaderID, "PROGRAM" );
	glDeleteShader( vertShader );
	glDeleteShader( fragShader );
	if (geomSource != nullptr)
	{
		glDeleteShader( geomShader );
	}
}

void Shader::setFloat( const GLchar *name, GLfloat value, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform1f( glGetUniformLocation( this->shaderID, name ), value );
}
void Shader::setInteger( const GLchar *name, GLint value, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform1i( glGetUniformLocation( this->shaderID, name ), value );
}
void Shader::setVector2f( const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform2f( glGetUniformLocation( this->shaderID, name ), x, y );
}
void Shader::setVector2f( const GLchar *name, const glm::vec2 &value, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform2f( glGetUniformLocation( this->shaderID, name ), value.x, value.y );
}
void Shader::setVector3f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform3f( glGetUniformLocation( this->shaderID, name ), x, y, z );
}
void Shader::setVector3f( const GLchar *name, const glm::vec3 &value, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform3f( glGetUniformLocation( this->shaderID, name ), value.x, value.y, value.z );
}
void Shader::setVector4f( const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform4f( glGetUniformLocation( this->shaderID, name ), x, y, z, w );
}
void Shader::setVector4f( const GLchar *name, const glm::vec4 &value, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniform4f( glGetUniformLocation( this->shaderID, name ), value.x, value.y, value.z, value.w );
}
void Shader::setMatrix4( const GLchar *name, const glm::mat4 &matrix, GLboolean useShader )
{
	if (useShader)
		this->use();
	glUniformMatrix4fv( glGetUniformLocation( this->shaderID, name ), 1, GL_FALSE, glm::value_ptr( matrix ) );
}

void Shader::setVP(glm::mat4 view, glm::mat4 projection, GLboolean useShader )
{
	if (useShader)
		this->use();
	setMatrix4( "view", view );
	setMatrix4( "projection", projection );
}

void Shader::loadFromFile( const GLchar * vertFile, const GLchar * fragFile, const GLchar * geomFile)
{
	std::string vertCode;
	std::string fragCode;
	std::string geomCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::ifstream geomShaderFile;
	vertexShaderFile.exceptions( std::ifstream::badbit );
	fragmentShaderFile.exceptions( std::ifstream::badbit );
	geomShaderFile.exceptions( std::ifstream::badbit );
	try
	{
		vertexShaderFile.open( vertFile );
		fragmentShaderFile.open( fragFile );
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertCode = vShaderStream.str();
		fragCode = fShaderStream.str();
		if (geomFile != nullptr)
		{
			geomShaderFile.open( geomFile );
			std::stringstream gShaderStream;
			gShaderStream << geomShaderFile.rdbuf();
			geomShaderFile.close();
			geomCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "SHADER ERROR: FAILED TO READ SHADER FILE(S)" << std::endl;
	}
	const GLchar *vertShaderCode = vertCode.c_str();
	const GLchar *fragShaderCode = fragCode.c_str();
	const GLchar *geomShaderCode = geomCode.c_str();
	this->vertPath = vertFile;
	this->fragPath = fragFile;
	this->geomPath = geomFile;
	this->compile( vertShaderCode, fragShaderCode, geomFile != nullptr ? fragShaderCode : nullptr );
}

void Shader::checkCompileErrors( GLuint object, std::string type )
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv( object, GL_COMPILE_STATUS, &success );
		if (!success)
		{
			glGetShaderInfoLog( object, 1024, NULL, infoLog );
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv( object, GL_LINK_STATUS, &success );
		if (!success)
		{
			glGetProgramInfoLog( object, 1024, NULL, infoLog );
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Input/Input.h"
//#include "Engine.h"

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  3.0f;
const GLfloat SENSITIVTY =  0.15f;
const GLfloat DEFAULT_FOV =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
	GLfloat AdjustedMouseSensitivity;

public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
	GLfloat MaxFOV;
    GLfloat FOV;
	GLfloat multiplier = 1.0f;

    // Constructor with vectors
	Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), GLfloat yaw = YAW, GLfloat pitch = PITCH ) : Front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), MovementSpeed( SPEED ), MouseSensitivity( SENSITIVTY ), AdjustedMouseSensitivity( SENSITIVTY ), FOV( DEFAULT_FOV ), MaxFOV( DEFAULT_FOV )
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), AdjustedMouseSensitivity(SENSITIVTY), FOV(DEFAULT_FOV), MaxFOV( DEFAULT_FOV )
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

	glm::mat4 GetReverseViewMatrix()
	{
		return glm::lookAt( this->Position, this->Position - this->Front, this->Up );
	}

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(GLfloat deltaTime)
    {
		GLfloat velocity = this->MovementSpeed * deltaTime * multiplier;

		if (Input::keyIsHeld(GLFW_KEY_W))
			this->Position += this->Front * velocity;
		if (Input::keyIsHeld(GLFW_KEY_S))
			this->Position -= this->Front * velocity;
		if (Input::keyIsHeld(GLFW_KEY_A))
			this->Position -= this->Right * velocity;
		if (Input::keyIsHeld(GLFW_KEY_D))
			this->Position += this->Right * velocity;
		if (Input::keyIsHeld(GLFW_KEY_SPACE))
			this->Position.y += velocity;
		if (Input::keyIsHeld(GLFW_KEY_LEFT_CONTROL))
			this->Position.y -= velocity;
		if (Input::keyIsHeld(GLFW_KEY_LEFT_SHIFT))
			this->multiplier = 5.0f;
		else if (Input::keyIsHeld(GLFW_KEY_LEFT_ALT))
			this->multiplier = 0.5f;
		else
			this->multiplier = 1.0f;
    }

	GLboolean firstMouse = true;
	GLfloat lastX = 400.0f, lastY = 400.0f;
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLboolean constrainPitch = true)
    {
		std::pair<GLfloat, GLfloat> mousePos = Input::getMousePos();
		GLfloat xpos = mousePos.first;
		GLfloat ypos = mousePos.second;
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX;
		GLfloat yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		xoffset *= this->AdjustedMouseSensitivity;
		yoffset *= this->AdjustedMouseSensitivity;

        this->Yaw   += xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

	void resetMouse()
	{
		lastX = 400.0f, lastY = 400.0f;
		firstMouse = true;
	}

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll()
    {
		GLfloat yoffset = Input::getScrollOffset().second;
		if (this->FOV >= 1.0f && this->FOV <= this->MaxFOV){
			this->FOV -= yoffset;
			this->AdjustedMouseSensitivity = this->MouseSensitivity * (this->FOV / this->MaxFOV);
			if (AdjustedMouseSensitivity <= 0) {
				this->AdjustedMouseSensitivity = this->MouseSensitivity * (1.0f / this->MaxFOV);
			}
		}
        if (this->FOV <= 1.0f)
            this->FOV = 1.0f;
        if (this->FOV >= 45.0f)
            this->FOV = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
    }
};
#endif // !CAMERA_H
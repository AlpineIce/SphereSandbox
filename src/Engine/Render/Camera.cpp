#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

#define PI 3.14159265f

namespace Renderer
{

	Camera::Camera()
	{
		resolution = { 1, 1 };
		axes.cameraPitchYaw = { 180.0f, 0.0f };
		position = glm::vec3(0.0f, 0.0f, 2.0f);
	}

	Camera::~Camera()
	{
	}

	void Camera::setupCamera(float fov, unsigned int width, unsigned int height)
	{
		resolution = { width, height };
		projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
		updateView();
	}

	void Camera::translate(glm::vec3 translation)
	{
		position += translation;
		updateView();
	}

	void Camera::rotate(glm::vec2 pitchYaw)
	{
		float sensitivity = 0.15f;

		axes.cameraPitchYaw += pitchYaw * sensitivity;

		if (axes.cameraPitchYaw.x > 360.0f)
		{
			axes.cameraPitchYaw.x = axes.cameraPitchYaw.x - 360.0f;
		}
		else if (axes.cameraPitchYaw.x < -360.0f)
		{
			axes.cameraPitchYaw.x = axes.cameraPitchYaw.x + 360.0f;
		}
		if (axes.cameraPitchYaw.y > 89.0f)
		{
			axes.cameraPitchYaw.y = 89.0f;
		}
		else if (axes.cameraPitchYaw.y < -89.0f)
		{
			axes.cameraPitchYaw.y = -89.0f;
		}

		updateView();
	}

	void Camera::updateView()
	{

		axes.target.x = cos(axes.cameraPitchYaw.y * (PI / 180.0f)) * (sin(axes.cameraPitchYaw.x * (PI / 180.0f)));
		axes.target.y = sin(axes.cameraPitchYaw.y * (PI / 180.0f));
		axes.target.z = cos(axes.cameraPitchYaw.y * (PI / 180.0f)) * (cos(axes.cameraPitchYaw.x * (PI / 180.0f)));

		axes.direction = glm::normalize(position - axes.target);
		axes.rightAxis = glm::normalize(glm::cross(glm::vec3(0.0f, -1.0f, 0.0f), axes.target));
		axes.upAxis = glm::cross(axes.direction, axes.rightAxis);

		view = glm::lookAt(position, axes.target + position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
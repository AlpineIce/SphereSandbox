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
		cameraPitchYaw = { 180.0f, 0.0f };
		position = glm::vec3(0.0f, 0.0f, 2.0f);
		bForwards = false;
		bBackwards = false;
		bLeft = false;
		bRight = false;
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

	void Camera::update(const double& deltaTime)
	{
		if (bForwards)
		{
			translate(target * (float)(deltaTime * 8.0f));
		}
		if (bBackwards)
		{
			translate(-target * (float)(deltaTime * 8.0f));
		}
		if (bLeft)
		{
			translate(-rightAxis * (float)(deltaTime * 8.0f));
		}
		if (bRight)
		{
			translate(rightAxis * (float)(deltaTime * 8.0f));
		}
	}

	void Camera::translate(glm::vec3 translation)
	{
		position += translation;
		updateView();
	}

	void Camera::rotate(glm::vec2 pitchYaw)
	{
		float sensitivity = 0.15f;

		cameraPitchYaw += pitchYaw * sensitivity;

		if (cameraPitchYaw.x > 360.0f)
		{
			cameraPitchYaw.x = cameraPitchYaw.x - 360.0f;
		}
		else if (cameraPitchYaw.x < -360.0f)
		{
			cameraPitchYaw.x = cameraPitchYaw.x + 360.0f;
		}
		if (cameraPitchYaw.y > 89.0f)
		{
			cameraPitchYaw.y = 89.0f;
		}
		else if (cameraPitchYaw.y < -89.0f)
		{
			cameraPitchYaw.y = -89.0f;
		}

		updateView();
	}

	void Camera::updateView()
	{

		target.x = cos(cameraPitchYaw.y * (PI / 180.0f)) * (sin(cameraPitchYaw.x * (PI / 180.0f)));
		target.y = sin(cameraPitchYaw.y * (PI / 180.0f));
		target.z = cos(cameraPitchYaw.y * (PI / 180.0f)) * (cos(cameraPitchYaw.x * (PI / 180.0f)));

		direction = glm::normalize(position - target);
		rightAxis = glm::normalize(glm::cross(glm::vec3(0.0f, -1.0f, 0.0f), target));
		upAxis = glm::cross(direction, rightAxis);

		view = glm::lookAt(position, target + position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
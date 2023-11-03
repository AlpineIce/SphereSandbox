#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

#define PI 3.14159265

namespace Renderer
{

	Camera::Camera()
	{
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

	void Camera::setupCamera(float fov, int& width, int& height)
	{
		projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);
		updateView();
	}

	void Camera::update(const float& deltaTime)
	{
		if (bForwards)
		{
			translate(target * deltaTime * 5.0f);
		}
		if (bBackwards)
		{
			translate(-target * deltaTime * 5.0f);
		}
		if (bLeft)
		{
			translate(-rightAxis * deltaTime * 5.0f);
		}
		if (bRight)
		{
			translate(rightAxis * deltaTime * 5.0f);
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

		target.x = cos(cameraPitchYaw.y * (PI / 180.0)) * (sin(cameraPitchYaw.x * (PI / 180.0)));
		target.y = sin(cameraPitchYaw.y * (PI / 180.0));
		target.z = cos(cameraPitchYaw.y * (PI / 180.0)) * (cos(cameraPitchYaw.x * (PI / 180.0)));

		direction = glm::normalize(position - target);
		rightAxis = glm::normalize(glm::cross(glm::vec3(0.0f, -1.0f, 0.0f), target));
		upAxis = glm::cross(direction, rightAxis);

		view = glm::lookAt(position, target + position, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
#pragma once
#include "glm/glm.hpp"

namespace Renderer
{
	struct CameraAxes
	{
		glm::vec3 target;
		glm::vec3 direction;
		glm::vec3 upAxis;
		glm::vec3 rightAxis;
		glm::vec2 cameraPitchYaw;
	};

	class Camera
	{
	private:
		glm::uvec2 resolution;
		glm::vec3 position;
		CameraAxes axes; //angle of camera horizontal and vertical in degrees

		glm::mat4 projection;
		glm::mat4 view;

		void updateView();
	public:
		Camera();
		~Camera();

		void setupCamera(float fov, unsigned int width, unsigned int height);
		void translate(glm::vec3 translation); //translation is added to current translation
		void rotate(glm::vec2 offset);

		glm::vec3 getPosition() const { return position; }
		CameraAxes getAxes() const { return axes; }
		glm::mat4 getProjection() const { return projection; }
		glm::mat4 getView() const { return view; }
	};

}
#pragma once
#include <glm/glm.hpp>

namespace Renderer
{

	class Camera
	{
	private:
		glm::vec3 position;
		glm::vec3 target;
		glm::vec3 direction;
		glm::vec3 upAxis;
		glm::vec3 rightAxis;
		glm::vec2 cameraPitchYaw; //angle of camera horizontal and vertical in degrees

		glm::mat4 projection;
		glm::mat4 view;

		void updateView();
	public:
		Camera();
		~Camera();

		void setupCamera(float fov, int& width, int& height);
		void update(const float& deltaTime);

		void translate(glm::vec3 translation);
		void rotate(glm::vec2 offset);

		bool bForwards;
		bool bBackwards;
		bool bLeft;
		bool bRight;

		const glm::vec3& getPosition() { return position; }
		const glm::vec3& getTarget() { return target; }
		const glm::vec3& getDirection() { return direction; }

		const glm::mat4& getProjection() { return projection; }
		const glm::mat4& getView() { return view; }
	};

}
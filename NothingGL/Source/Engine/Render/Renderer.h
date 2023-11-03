#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "Camera.h"

namespace Renderer
{

	class RenderEngine
	{
	private:
		//----------"GLOBAL" VARIABLES----------//

		static Camera camera;
		static double deltaTime;
		static double time;
		static glm::dvec2 lastCursor;
		static glm::dvec2 pitchYaw;

		//----------GLFW----------//

		GLFWwindow* window;
		static void glfwErrorCallback(int error, const char* description);
		static void rebuildFramebufferCallback(GLFWwindow* window, int width, int height);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

		//----------OTHER STUFF----------//

		int initWindow(int width, int height);


	public:
		RenderEngine(int width, int height);
		~RenderEngine();

		void preDrawEvent();
		void postDrawEvent();
		bool checkWindowClose() { return glfwWindowShouldClose(window); }

		const double& getDeltaTime();
		void printTime();

	};

}
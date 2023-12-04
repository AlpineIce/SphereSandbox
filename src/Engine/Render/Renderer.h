#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

		GLFWwindow* window;
		static void glfwErrorCallback(int error, const char* description);
		static void rebuildFramebufferCallback(GLFWwindow* window, int width, int height);

		int initWindow(int width, int height);

	public:
		RenderEngine(int width, int height);
		~RenderEngine();

		void preDrawEvent();
		void postDrawEvent();
		bool checkWindowClose() { return glfwWindowShouldClose(window); }

		inline double& getDeltaTime() const { return deltaTime; }
		inline Camera* getCamera() const { return &camera; }
		inline double* getTimePointer() const { return &time; }
		inline GLFWwindow* getWindowPtr() const {return window; }
	};

}
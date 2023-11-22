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
		static bool mouseClickEvent;
		static glm::dvec2 lastCursor;
		static glm::dvec2 pitchYaw;

		//----------GLFW----------//

		GLFWwindow* window;
		static void glfwErrorCallback(int error, const char* description);
		static void rebuildFramebufferCallback(GLFWwindow* window, int width, int height);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

		//----------OTHER STUFF----------//

		int initWindow(int width, int height);


	public:
		RenderEngine(int width, int height);
		~RenderEngine();

		void preDrawEvent();
		void postDrawEvent();
		bool checkWindowClose() { return glfwWindowShouldClose(window); }

		inline double& getDeltaTime() const { return deltaTime; }
		inline Camera* getCamera() const { return &camera; }
		inline double* getTimePointer() const { return &time; };

		bool* getClickEvent() { return &mouseClickEvent; }


	};

}
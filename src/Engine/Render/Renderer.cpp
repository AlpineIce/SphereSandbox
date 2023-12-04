#include "Renderer.h"
#include "Material.h"
#include <iostream>
#include "Camera.h"


namespace Renderer
{

	double RenderEngine::deltaTime;
	Camera RenderEngine::camera;
	double RenderEngine::time;

	RenderEngine::RenderEngine(int width, int height)
	{
		initWindow(width, height);

		time = 0.0f;
	}

	RenderEngine::~RenderEngine()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	//----------CALLBACK FUNCTIONS----------//

	void RenderEngine::glfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
	}

	void RenderEngine::rebuildFramebufferCallback(GLFWwindow* window, int width, int height)
	{
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		camera.setupCamera(90.0f, (unsigned int)width, (unsigned int)height);
	}

	//----------GLFW WINDOW----------//

	void GLAPIENTRY MessageCallback(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	int RenderEngine::initWindow(int width, int height)
	{
		if (glfwInit() != GLFW_TRUE)
		{
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		//window creation, context, error callback
		window = glfwCreateWindow(width, height, "TODO CREATE TITLE", NULL, NULL);
		if (!window)
		{
			return -1;
		}

		glfwMakeContextCurrent(window);
		glViewport(0, 0, width, height);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetErrorCallback(glfwErrorCallback);
		glfwSetFramebufferSizeCallback(window, rebuildFramebufferCallback);

		//glew
		if (glewInit() != GLEW_OK)
		{
			return -1;
		}

		camera.setupCamera(90.0f, width, height);

		// During init, enable debug output
		/*
#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
#endif*/

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		return 0;
	}

	void RenderEngine::preDrawEvent()
	{
		//glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderEngine::postDrawEvent()
	{
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}


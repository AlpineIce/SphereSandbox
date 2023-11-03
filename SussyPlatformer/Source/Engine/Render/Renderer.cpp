#include "Renderer.h"
#include <iostream>
#include <fstream>
#include "Material.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Renderer
{

	double RenderEngine::deltaTime;
	Camera RenderEngine::camera;
	glm::dvec2 RenderEngine::lastCursor = { 0.0f, 0.0f };
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
		camera.setupCamera(90.0f, width, height);
	}

	void RenderEngine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && camera.bForwards == false)
			camera.bForwards = true;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && camera.bBackwards == false)
			camera.bBackwards = true;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && camera.bLeft == false)
			camera.bLeft = true;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && camera.bRight == false)
			camera.bRight = true;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
			camera.bForwards = false;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
			camera.bBackwards = false;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
			camera.bLeft = false;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
			camera.bRight = false;
	}

	void RenderEngine::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		glm::dvec2 offset;
		offset.x = (lastCursor.x - xpos);
		offset.y = (lastCursor.y - ypos); // reversed since y-coordinates range from bottom to top

		lastCursor.x = xpos;
		lastCursor.y = ypos;

		camera.rotate(offset);

		//camera.rotate()
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

	void RenderEngine::printTime()
	{
		std::cout << getDeltaTime() << std::endl;
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
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPosCallback(window, mouse_callback);

		//glew
		if (glewInit() != GLEW_OK)
		{
			return -1;
		}

		camera.setupCamera(90.0f, width, height);

		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);

		//load shader
		Material cupBaseMaterial(MaterialType::PBR, "Materials/SkulCup/m_Base");
		Material cupBoneMaterial(MaterialType::PBR, "Materials/SkulCup/m_Bone");
		Material cupGemMaterial(MaterialType::PBR, "Materials/SkulCup/m_Gem");
		Material cupTeethMaterial(MaterialType::PBR, "Materials/SkulCup/m_Teeth");

		//load models
		Model model("Models/SculCup/SkullCup.fbx");
		Model ground("Models/Ground/ground.fbx");
		Model cube("Models/Cube/Cube.fbx");
		ModelInstance testModel(&model);
		ModelInstance testModel2(&model);
		ModelInstance ground1(&ground);
		ModelInstance bruhcube(&cube);

		Transformation cubeTransform;
		cubeTransform.location = { 5.0f, 0.0f, 0.0f };
		bruhcube.setTransformation(cubeTransform);
		

		Transformation groundTransform;
		groundTransform.rotation = glm::quat(0.707106f, -0.707106f, 0.0f, 0.0f);
		groundTransform.location = (glm::vec3(0.0f, -4.0f, 0.0f));
		ground1.setTransformation(groundTransform);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		while (!glfwWindowShouldClose(window))
		{
			Transformation lightTransform;
			lightTransform.location = { sin(time) * 5.0f, 0.0f, cos(time) * 5.0f };
			cupBaseMaterial.getShader()->setFloat3("lightPos", lightTransform.location);

			Transformation modelTransform;
			modelTransform.location = glm::vec3(sin(time) * 2.0f, cos(time) * 2.0f, 0.0f);
			modelTransform.rotation = glm::quat(sin(time), 0.0f, cos(time), 0.0f);


			preDrawEvent();
			ground1.render(camera);
			testModel.setTransformation(modelTransform);
			testModel.render(camera);
			testModel2.render(camera);
			bruhcube.render(camera);

			//model.draw(shader, camera);
			//ground.draw(shader, camera);

			postDrawEvent();
		}

		return 0;
	}

	void RenderEngine::preDrawEvent()
	{
		glClearColor(0.0f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderEngine::postDrawEvent()
	{
		deltaTime = glfwGetTime() - time;
		time = glfwGetTime();
		camera.update(deltaTime);

		auto err = glGetError();
		if (err)
		{
			std::cout << err << std::endl;
		}

		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	const double& RenderEngine::getDeltaTime()
	{
		return deltaTime;
	}

}


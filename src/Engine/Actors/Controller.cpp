#include "Controller.h"
#include "GLFW/glfw3.h"

namespace Actor
{
    glm::dvec2 Controller::lastCursor = { 0.0f, 0.0f };
    glm::dvec2 Controller::curCursor = { 0.0f, 0.0f };
	bool Controller::mouseClickEvent = false;
    bool Controller::bForwards = false;
	bool Controller::bBackwards = false;
	bool Controller::bLeft = false;
	bool Controller::bRight = false;

    Controller::Controller(Engine* engine)
        :Actor(engine), camera(engine)
    {
        glfwSetKeyCallback(engine->getRenderer()->getWindowPtr(), keyCallback);
		glfwSetMouseButtonCallback(engine->getRenderer()->getWindowPtr(), mouse_button_callback);
		glfwSetCursorPosCallback(engine->getRenderer()->getWindowPtr(), mouse_callback);

        bForwards = false;
		bBackwards = false;
		bLeft = false;
		bRight = false;
    }

    Controller::~Controller()
    {
    }

    //----------INPUT---------//

    void Controller::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			bForwards = true;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			bBackwards = true;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			bLeft = true;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			bRight = true;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
			bForwards = false;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
			bBackwards = false;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
			bLeft = false;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
			bRight = false;


	}

	void Controller::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			mouseClickEvent = true;
            
	}

	void Controller::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		curCursor.x = (xpos);
		curCursor.y = (ypos);
	}

    void Controller::updateController()
    {
        //Camera movement
        glm::dvec2 rotOffset = lastCursor - curCursor;

        camera.rotate(rotOffset);
        lastCursor = curCursor;
        
        glm::vec3 target = engine->getRenderer()->getCamera()->getAxes().target;
        glm::vec3 rightAxis = engine->getRenderer()->getCamera()->getAxes().rightAxis;

        //TODO normalize speed
        if (bForwards)
		{
			camera.offsetMove(target * (float)(engine->getRenderer()->getDeltaTime() * 8.0f));
        }
		if (bBackwards)
		{
			camera.offsetMove(-target * (float)(engine->getRenderer()->getDeltaTime() * 8.0f));
		}
		if (bLeft)
		{
			camera.offsetMove(-rightAxis * (float)(engine->getRenderer()->getDeltaTime() * 8.0f));
		}
		if (bRight)
		{
			camera.offsetMove(rightAxis * (float)(engine->getRenderer()->getDeltaTime() * 8.0f));
		}
    }
}

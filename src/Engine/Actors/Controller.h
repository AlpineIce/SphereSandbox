#pragma once
#include "Actor.h"
#include "Components/CameraComponent.h"

//this class can be viewed as a character or literally a "controller".
//includes a camera

namespace Actor
{
    class Controller : public Actor
    {
    private:
        ActorComponent::CameraComponent camera;

        //input
        static bool mouseClickEvent;
		static glm::dvec2 lastCursor;
        static glm::dvec2 curCursor;
		static glm::dvec2 pitchYaw;

        static bool bForwards;
		static bool bBackwards;
		static bool bLeft;
		static bool bRight;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    public:
        Controller(Engine* engine);
        virtual ~Controller() override;

        void updateController();
    };
}
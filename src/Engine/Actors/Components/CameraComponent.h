#pragma once
#include "Render/Camera.h"
#include "Engine.h"

namespace ActorComponent
{
    class CameraComponent
    {
    private:
        Renderer::Camera* camera;
        //todo a whole bunch of glfw input handling stuff

    public:
        CameraComponent(Engine* engine);
        ~CameraComponent();

        void offsetMove(glm::vec3 offset);
        void rotate(glm::vec2 offset);

        Renderer::CameraAxes getAxes() { return camera->getAxes(); }

    };
}
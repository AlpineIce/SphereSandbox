#include "CameraComponent.h"

namespace ActorComponent
{
    CameraComponent::CameraComponent(Engine* engine)
    {
        camera = engine->getRenderer()->getCamera();
    }

    CameraComponent::~CameraComponent()
    {

    }

    void CameraComponent::offsetMove(glm::vec3 offset)
    {
        camera->translate(offset);
    }

    void CameraComponent::rotate(glm::vec2 offset)
    {
        camera->rotate(offset);
    }
}
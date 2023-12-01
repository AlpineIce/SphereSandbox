#include "Controller.h"
#include "GLFW/glfw3.h"

namespace Actor
{
    Controller::Controller()
    {
    }

    Controller::~Controller(Engine* engine)
        :Actor(engine)
    {
        camera = engine->getCamera();
    }
}

#include "Actor.h"

//this class can be viewed as a character or literally a "controller".
//includes the renderer camera in it and has variables for controlling it.

namespace Actor
{
    class Controller
    {
    private:
        Renderer::Camera* camera;
        //todo a whole bunch of glfw input handling stuff

    public:
        Controller(Engine* engine);
        ~Controller();
    };
}
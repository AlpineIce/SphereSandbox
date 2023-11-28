#include "Actor.h"
#include "Render/Light.h"

namespace Actor
{

    class LightActor : public Actor
    {
    private:
        unsigned int engineIndex = 0;
        Light::LightType type;
        Engine* engine;

        Light::Light* lightPtr; //raw ptr

    public:
        LightActor(Engine* engine, Light::LightType type, Light::ShaderStruct lightInfo);
        virtual ~LightActor() override;

        inline Light::ShaderStruct getInfo() const { return lightPtr->getShaderStruct(); }
        void setNewInfo(Light::ShaderStruct info);
        void setDirectionalRotation(float horizontalAngle, float verticalAngle); //angles in degrees
    };

}

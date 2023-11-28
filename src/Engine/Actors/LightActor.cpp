#include "LightActor.h"

namespace Actor
{

    LightActor::LightActor(Engine* engine, Light::LightType type, Light::ShaderStruct lightInfo )
        :Actor(engine), type(type)
    {
        switch(type)
        {
        case Light::LightType::DIRECTIONAL:
            lightPtr = new Light::DirectionalLight(lightInfo);
            engine->setDirectionalLight((Light::DirectionalLight*)lightPtr);

            break;

        case Light::LightType::AMBIENT:
            lightPtr = new Light::AmbientLight(lightInfo);
            engine->setAmbientLight((Light::AmbientLight*)lightPtr);


            break;

        case Light::LightType::POINT:
            lightPtr = new Light::PointLight(lightInfo);
            engineIndex = engine->addPointLightPtr((Light::PointLight*)lightPtr);

            break;

        }
    }

    LightActor::~LightActor()
    {
        switch(this->type)
        {
        case Light::LightType::DIRECTIONAL:
            if (engine)
            {
                //engine->setDirectionalLight(nullptr);     segfault for whatever reason 
            }
            delete lightPtr;

            break;

        case Light::LightType::AMBIENT:
            if (engine)
            {
                engine->setAmbientLight(nullptr);
            }
            delete lightPtr;

            break;

        case Light::LightType::POINT:
            if (engine)
            {
                engine->removePointLightPtr(engineIndex);
            }
            delete lightPtr;

            break;

        }
    }

    void LightActor::setNewInfo(Light::ShaderStruct info)
    {
        lightPtr->setShaderStruct(info);
    }

    void LightActor::setDirectionalRotation(float horizontalAngle, float verticalAngle)
    {
        Light::ShaderStruct newInfo = lightPtr->getShaderStruct();
		newInfo.position.x = sin(glm::radians(horizontalAngle));
		newInfo.position.y = sin(glm::radians(verticalAngle));
		newInfo.position.z = cos(glm::radians(horizontalAngle));
        lightPtr->setShaderStruct(newInfo);
	}
}


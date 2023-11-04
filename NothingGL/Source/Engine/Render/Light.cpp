#include "Light.h"


//----------BASE LIGHT DEFINITIONS----------//

Light::Light(glm::vec3 position)
	:color(1.0f), power(100.0f), position(position)
{

}

Light::~Light()
{
}

//----------DIRECTIONAL LIGHT DEFINITIONS----------//

DirectionalLight::DirectionalLight(glm::vec3 position)
	:Light(position)
{
}

DirectionalLight::~DirectionalLight()
{
}

//----------POINT LIGHT DEFINITIONS----------//

PointLight::PointLight(glm::vec3 position)
	:Light(position)
{
}

PointLight::~PointLight()
{
}

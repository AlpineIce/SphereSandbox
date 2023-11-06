#include "Light.h"


//----------BASE LIGHT DEFINITIONS----------//

Light::Light(glm::vec3 position)
{
	light.color = glm::vec3(1.0f);
	light.power = 10.0f;
	light.position = position;
}

Light::~Light()
{
}

//----------DIRECTIONAL LIGHT DEFINITIONS----------//

DirectionalLight::DirectionalLight()
	:Light(glm::vec3(0.0, 1.0, 1.0f)) //directional light pointing straight down on init
{
	light.power = 20.0f; //20 is the default value, since this value gets divided by 20 when sent to the shader uniform
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setRotation(float horizontalAngle, float verticalAngle) 
{
	
	light.position.x = sin(glm::radians(horizontalAngle));
	light.position.y = sin(glm::radians(verticalAngle));
}

//----------POINT LIGHT DEFINITIONS----------//

PointLight::PointLight(glm::vec3 position)
	:Light(position)
{
}

PointLight::~PointLight()
{
}

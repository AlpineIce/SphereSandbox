#include "Light.h"


//----------BASE LIGHT DEFINITIONS----------//

Light::Light(glm::vec3 position)
{
	light.color = glm::vec3(1.0f);
	light.power = 20.0f;
	light.position = position;
}

Light::~Light()
{
}

//----------DIRECTIONAL LIGHT DEFINITIONS----------//

DirectionalLight::DirectionalLight()
	:Light(glm::vec3(0.0, 1.0, 1.0f)) //directional light pointing straight down on init
{
	light.power = 3.0f;
}

DirectionalLight::DirectionalLight(glm::vec3 color)
	:Light(glm::vec3(0.0, 1.0, 1.0f)) //directional light pointing straight down on init
{
	light.power = 3.0f;
	light.color = color;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::setRotation(float horizontalAngle, float verticalAngle) 
{
	
	light.position.x = sin(glm::radians(horizontalAngle));
	light.position.y = sin(glm::radians(verticalAngle));
	light.position.z = cos(glm::radians(horizontalAngle));
}

//----------POINT LIGHT DEFINITIONS----------//

PointLight::PointLight(glm::vec3 position)
	:Light(position)
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 color)
	:Light(position)
{
	light.color = color;
}

PointLight::~PointLight()
{
}

#include "Light.h"

namespace Light
{

	//----------BASE LIGHT DEFINITIONS----------//

	Light::Light(ShaderStruct lightInfo)
		:light(lightInfo)
	{
	}

	Light::~Light()
	{
	}

	//----------DIRECTIONAL LIGHT DEFINITIONS----------//

	DirectionalLight::DirectionalLight(ShaderStruct lightInfo)
		:Light(lightInfo) //directional light pointing straight down on init
	{
	}

	DirectionalLight::~DirectionalLight()
	{
	}

	//----------POINT LIGHT DEFINITIONS----------//

	PointLight::PointLight(ShaderStruct lightInfo)
		:Light(lightInfo)
	{
	}

	PointLight::~PointLight()
	{
	}

	//----------AMBIENT LIGHT DEFINITIONS----------//

	AmbientLight::AmbientLight(ShaderStruct lightInfo)
		:Light(lightInfo)
	{
	}

	AmbientLight::~AmbientLight()
	{
	}

	glm::vec3 AmbientLight::getAmbientLight() const
	{
		return lightInfo.color * lightInfo.power * 0.1f; //multiply by 0.1 to bring down the ambient light so that strength is a more realistic variable
	}

}

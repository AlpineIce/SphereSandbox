#pragma once
#include "GLM/glm.hpp"

class Light
{
protected:
	struct ShaderStruct
	{
		float power;
		glm::vec3 position; //position is actually rotation for directional lights
		glm::vec3 color;
	};

	ShaderStruct light;

public:
	Light(glm::vec3 position);
	virtual ~Light();

	virtual ShaderStruct getShaderStruct() { return light; }
};

class DirectionalLight : public Light
{
private:

public:
	DirectionalLight();
	~DirectionalLight() override;

	void setRotation(float horizontalAngle, float verticalAngle); //angles in degrees
};

class PointLight : public Light
{
private:

public:
	PointLight(glm::vec3 position);
	~PointLight() override;

};
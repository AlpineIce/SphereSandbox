#pragma once
#include "GLM/glm.hpp"

class Light
{
protected:
	float power;
	glm::vec3 position;
	glm::vec3 color;

public:
	Light(glm::vec3 position);
	virtual ~Light();
};

class DirectionalLight : public Light
{
private:

public:
	DirectionalLight(glm::vec3 position);
	~DirectionalLight() override;
};

class PointLight : public Light
{
private:

public:
	PointLight(glm::vec3 position);
	~PointLight() override;
};
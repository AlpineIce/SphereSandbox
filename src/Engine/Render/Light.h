#pragma once
#include "glm/glm.hpp"

namespace Light
{

	enum LightType
	{
		POINT = 0,
		DIRECTIONAL = 1,
		AMBIENT = 2
	};

	struct ShaderStruct
	{
		float power = 1.0f;
		glm::vec3 position = glm::vec3(0.0f); //position is actually rotation for directional lights, ignored for ambient
		glm::vec3 color = glm::vec3(1.0f);
	};

	class Light
	{
	protected:
		ShaderStruct light;

	public:
		Light(ShaderStruct lightInfo);
		virtual ~Light();

		inline virtual ShaderStruct getShaderStruct() const { return light; }
		inline virtual void setPower(float power) { light.power = power; }
		inline virtual void setShaderStruct(ShaderStruct info) { this->light = info; }
	};

	class DirectionalLight : public Light
	{
	private:

	public:
		DirectionalLight(ShaderStruct lightInfo);
		~DirectionalLight() override;
	};

	class PointLight : public Light
	{
	private:

	public:
		PointLight(ShaderStruct lightInfo);
		~PointLight() override;

		void setPosition(glm::vec3 position) { light.position = position; }

	};

	class AmbientLight : public Light
	{
	private:
		ShaderStruct lightInfo;

	public:
		AmbientLight(ShaderStruct lightInfo);
		~AmbientLight();

		glm::vec3 getAmbientLight() const;
		inline void setPower(float power) { lightInfo.power = power; }
	};

}
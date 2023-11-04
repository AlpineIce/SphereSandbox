#pragma once
#include <string>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "Texture.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"


enum MaterialType
{
	PBR = 0
};

enum UniformVariable
{
	ALBEDO = 0,
	SPECULAR = 1,
	ROUGHNESS = 2,
	NORMAL = 3
};

namespace Renderer
{

	class Shader
	{
	private:
		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shader;

		std::string vertexData;
		std::string fragmentData;

		int loadShader(const char* vertexPath, const char* fragmentPath);

	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		~Shader();

		void bind();
		void unbind();

		void setFloat(const std::string& name, const float& value);
		void setFloat2(const std::string& name, const glm::vec2& value);
		void setFloat3(const std::string& name, const glm::vec3& value);
		void setFloat4(const std::string& name, const glm::vec4& value);

		void setMat3(const std::string& name, const glm::mat3& value);
		void setMat4(const std::string& name, const glm::mat4& value);

		void setInt(const std::string& name, const int& value);
		void setBool(const std::string& name, const bool& value);

		//void setMat3(const std::string& name, glm::mat3& value);
		//void setMat4(const std::string& name, glm::mat4& value);

		std::vector<std::string> getUniforms();
		std::vector<std::string> getAttributes();
		const unsigned int& getShaderID();

	};

	class Material
	{
	private:
		Shader* shader;
		std::map<TextureType, std::shared_ptr<Texture>> textures;
		std::map<UniformVariable, std::string> uniforms;

		void getUniforms(MaterialType type);
		void loadTextures(std::string path);
		
	public:
		Material(MaterialType type, std::string path, Shader* shaderBase);
		~Material();

		void useMaterial();
		void unbind();

		inline Shader* getShader() { return shader; }
	};

}
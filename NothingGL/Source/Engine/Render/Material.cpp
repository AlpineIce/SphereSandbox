#include "Material.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace Renderer
{

	unsigned int Material::defaultTex = 0;
	unsigned int Material::defaultAlbedo = 0;

	//----------SHADER DEFINITIONS----------//

	Shader::Shader(const char* vertexPath, const char* fragmentPath, Camera* camera)
		:cameraPtr(camera)
	{
		if (loadShader(vertexPath, fragmentPath) == -1)
		{
			throw std::runtime_error("Error: shader couldn't be loaded");
		}
	}

	Shader::~Shader()
	{
		glDeleteProgram(shader);
	}

	int Shader::checkUniform(std::string name)
	{
		int location = glGetUniformLocation(shader, name.data());
		if (location == -1)
		{
			std::cout << "Warning: uniform \"" << name << "\" doesn't exist in shader" << "\n";
			return -1;
		}
		return location;
	}

	int Shader::loadShader(const char* vertexPath, const char* fragmentPath)
	{
		//opening and copying files to strings
		std::ifstream vertex(vertexPath);
		if (!vertex.is_open()) {
			return -1;
		}

		std::ifstream fragment(fragmentPath);
		if (!fragment.is_open()) {
			return -1;
		}

		std::stringstream vertexSS;
		std::stringstream fragmentSS;
		vertexSS << vertex.rdbuf();
		fragmentSS << fragment.rdbuf();

		vertexData = vertexSS.str();
		fragmentData = fragmentSS.str();


		vertex.close();
		fragment.close();

		//vertex shader
		const char* vertexSource = vertexData.data();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "Error: couldnt compile vertex shader\n" << infoLog << std::endl;
		}

		//fragment shader
		const char* fragmentSource = fragmentData.data();
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "Error: couldnt compile fragment shader\n" << infoLog << std::endl;
		}


		//create shader program
		shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);

		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Error: couldnt link shaders\n" << infoLog << std::endl;
		}

		glDetachShader(shader, vertexShader);
		glDetachShader(shader, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return 0;
	}

	void Shader::bind()
	{
		glUseProgram(shader);
	}

	void Shader::unbind()
	{
		glUseProgram(0);
	}

	void Shader::setCameraPosition(glm::vec3 position)
	{
		setFloat3("viewPos", position);
	}

	void Shader::updateLights(const std::vector<std::shared_ptr<PointLight>>& lights, const std::shared_ptr<DirectionalLight>& directLight)
	{
		const unsigned int MAX_POINT_LIGHTS = 4; //relates to the defined maximum point lights in the PBR shader
		unsigned int i = 0;
		
		for (std::shared_ptr<PointLight> light : lights)
		{
			if (light.get() != NULL && i < MAX_POINT_LIGHTS) //point light must be valid
			{
				setFloat3(std::string("pLights[") + std::to_string(i) + std::string("].pos"), light->getShaderStruct().position);
				setFloat3(std::string("pLights[") + std::to_string(i) + std::string("].color"), light->getShaderStruct().color);
				setFloat(std::string("pLights[") + std::to_string(i) + std::string("].power"), light->getShaderStruct().power);
				setFloat(std::string("pLights[") + std::to_string(i) + std::string("].constant"), 1.0f);	//TODO FIGURE OUT WHY THE LAST 3 VARIABLES WONT WORK AT ALL
				setFloat(std::string("pLights[") + std::to_string(i) + std::string("].linear"), 0.05f);		
				setFloat(std::string("pLights[") + std::to_string(i) + std::string("].quad"), 0.0f);
				i++;
			}
			else
			{
				std::cout << "Warning: couldnt calculate all point lights in scene. Rewrite your terrible code to work properly please" << std::endl;
			}
		}

		if (directLight.get() != NULL) //directional light must be valid
		{
			setFloat3("sun.rot", directLight->getShaderStruct().position); //note that position for directional light is actually rotation
			setFloat3("sun.color", directLight->getShaderStruct().color);
			setFloat("sun.power", directLight->getShaderStruct().power);
		}
	}

	void Shader::setFloat(const std::string& name, const float& value)
	{
		int location = checkUniform(name);
		glUniform1f(location, value);
	}

	void Shader::setFloat2(const std::string& name, const glm::vec2& value)
	{
		int location = checkUniform(name);
		glUniform2f(location, value.x, value.y);
	}

	void Shader::setFloat3(const std::string& name, const glm::vec3& value)
	{
		int location = checkUniform(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::setFloat4(const std::string& name, const glm::vec4& value)
	{
		int location = checkUniform(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::setMat3(const std::string& name, const glm::mat3& value)
	{
		int location = checkUniform(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::setMat4(const std::string& name, const glm::mat4& value)
	{
		int location = checkUniform(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	}

	void Shader::setInt(const std::string& name, const int& value)
	{
		int location = checkUniform(name);
		glUniform1i(location, value);
	}

	void Shader::setBool(const std::string& name, const bool& value)
	{
		int location = checkUniform(name);
		glUniform1i(location, value);
	}

	std::vector<std::string> Shader::getUniforms() //stack overflow code from someone just like the function below
	{
		GLint i;
		GLint count;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		std::vector<std::string> uniforms;
		glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++)
		{
			const GLsizei bufSize = 20;
			GLchar name[bufSize];
			glGetActiveUniform(shader, (GLuint)i, bufSize, nullptr, &size, &type, name);
			uniforms.push_back(std::string(name));
		}

		return uniforms;
	}

	std::vector<std::string> Shader::getAttributes() //80% of this definitely didnt come from stack overflow; also its not being used for anything currently
	{
		bind();

		GLint i;
		GLint count;
		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		std::vector<std::string> uniforms;
		glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &count);

		for (i = 0; i < count; i++)
		{
			const GLsizei bufSize = 16;
			GLchar name[bufSize];
			glGetActiveUniform(shader, (GLuint)i, bufSize, nullptr, &size, &type, name);
			uniforms.push_back(std::string(name));
		}

		unbind();

		return uniforms;
	}

	const unsigned int& Shader::getShaderID()
	{
		return shader;
	}

	//----------MATERIAL DEFINITIONS----------//

	

	Material::Material(MaterialType type, std::string path, Shader* shader)
		: shader(shader)
	{
		if (!defaultTex)
		{
			GLubyte imageData[3] = { 0, 0, 0 };
			glCreateTextures(GL_TEXTURE_2D, 1, &defaultTex);
			glBindTexture(GL_TEXTURE_2D, defaultTex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		if (!defaultAlbedo)
		{
			GLubyte imageData[3] = { 255, 255, 255 };
			glCreateTextures(GL_TEXTURE_2D, 1, &defaultAlbedo);
			glBindTexture(GL_TEXTURE_2D, defaultAlbedo);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		setTextureUniforms(type);
		loadTextures(path);
	}

	Material::~Material()
	{
	}

	void Material::setTextureUniforms(MaterialType type)
	{
		if (type == MaterialType::PBR)
		{
			shader->bind();
			std::vector<std::string> uniforms = shader->getUniforms();
			for (int i = 0; i < uniforms.size(); i++)
			{
				//textures
				auto a = uniforms.at(i);
				if (uniforms.at(i) == "Albedo")
				{
					this->uniforms[UniformVariable::ALBEDO] = uniforms.at(i);
				}
				else if (uniforms.at(i) == "Metallic")
				{
					this->uniforms[UniformVariable::SPECULAR] = uniforms.at(i);
				}
				else if (uniforms.at(i) == "Roughness")
				{
					this->uniforms[UniformVariable::ROUGHNESS] = uniforms.at(i);
				}
				else if (uniforms.at(i) == "Normal")
				{
					this->uniforms[UniformVariable::NORMAL] = uniforms.at(i);
				}
			}
			shader->unbind();
		}
		else
		{
			std::cout << "only material type is currently PBR, no material will be created" << std::endl;
			throw std::runtime_error("bruh");
		}
	}

	void Material::loadTextures(std::string path)
	{
		try
		{

			std::filesystem::directory_iterator directoryIterator(path);
			for(auto entry : directoryIterator)
			{
				std::string entryPath = entry.path().string();
				while (entryPath.find("\\") != std::string::npos)
				{
					entryPath.replace(entryPath.find("\\"), sizeof("\\") - 1, "/");
				}
				if (entryPath.find(".")) //make sure its an actual file and not a directory only
				{
					if (entry.path().string().find("Albedo") != std::string::npos && uniforms.count(UniformVariable::ALBEDO))
					{
						textures[UniformVariable::ALBEDO] = std::make_shared<Texture>(entryPath, UniformVariable::ALBEDO);
						continue;
					}
					if ((entry.path().string().find("Specular") != std::string::npos || entry.path().string().find("Metallic") != std::string::npos) && uniforms.count(UniformVariable::SPECULAR))
					{
						textures[UniformVariable::SPECULAR] = std::make_shared<Texture>(entryPath, UniformVariable::SPECULAR);
						continue;
					}
					if (entry.path().string().find("Roughness") != std::string::npos && uniforms.count(UniformVariable::ROUGHNESS))
					{
						textures[UniformVariable::ROUGHNESS] = std::make_shared<Texture>(entryPath, UniformVariable::ROUGHNESS);
						continue;
					}
					if (entry.path().string().find("Normal") != std::string::npos && uniforms.count(UniformVariable::NORMAL))
					{
						textures[UniformVariable::NORMAL] = std::make_shared<Texture>(entryPath, UniformVariable::NORMAL);
						continue;
					}
				}
			}
		}
		catch (std::filesystem::filesystem_error error)
		{
			throw error.what();
		}
	}

	void Material::useMaterial()
	{
		shader->bind();
		
		glActiveTexture(GL_TEXTURE0);						//default albedo color
		glBindTexture(GL_TEXTURE_2D, defaultAlbedo);
		if (uniforms.count(UniformVariable(0)))
		{
			shader->setInt(uniforms.at(UniformVariable(0)), 0);
		}

		for (int i = 1; i < MAX_TEXTURE_COUNT; i++) //clear all textures out 
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, defaultTex);
			if (uniforms.count(UniformVariable(i)))
			{
				shader->setInt(uniforms.at(UniformVariable(i)), i);
			}
			
		}
		unsigned int texIndex = 0;
		for (auto [key, val] : textures) //set which textures are loaded for the material
		{
			val->useTexture(texIndex);
			shader->setInt(uniforms.at(UniformVariable(key)), texIndex);
			
			texIndex++;
		}
		
	}

	void Material::unbind()
	{
		for (auto [key, val] : textures)
		{
			val->unbind();
		}
		shader->unbind();
	}
}
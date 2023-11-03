#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>

namespace Renderer
{

	enum imageDataType
	{
		RGBA = GL_RGBA8,
		RGB = GL_RGB8,
		RG = GL_RG8,
		R = GL_R8
	};

	enum TextureType
	{
		ALBEDO = 0,
		SPECULAR = 1,
		ROUGHNESS = 2,
		NORMAL = 3
	};

	class Texture
	{
	private:
		unsigned int texObj;
		unsigned char* imageData;
		int resX, resY;
		int nrChannels;
		imageDataType imageFormat;
		TextureType type;
		std::string path;

		void createTexture(std::string filename, TextureType type);
		void loadTexture(const char* filename);

	public:
		Texture(std::string filename, TextureType type);
		~Texture();
		
		void useTexture(unsigned int texIndex);
		void unbind();

		inline const unsigned int& getID() { return texObj; }
		inline TextureType getType() { return type; }
		inline const std::string getPath() { return path; };
	};

}
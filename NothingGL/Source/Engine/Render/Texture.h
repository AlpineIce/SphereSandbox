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

	class Texture
	{
	private:
		unsigned int texObj;
		unsigned char* imageData;
		int resX, resY;
		int nrChannels;
		imageDataType imageFormat;
		int type;
		std::string path;

		void createTexture(std::string filename, int type);
		void loadTexture(const char* filename);

	public:
		Texture(std::string filename, int type);
		~Texture();
		
		void useTexture(unsigned int texIndex);
		void unbind();

		inline const unsigned int& getID() { return texObj; }
		inline int getType() { return type; }
		inline const std::string getPath() { return path; };
	};

}
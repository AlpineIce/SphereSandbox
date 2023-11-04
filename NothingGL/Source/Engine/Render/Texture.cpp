#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#include <iostream>

namespace Renderer
{

	Texture::Texture(std::string filename, int type)
		:type(type), path(filename)
	{
		createTexture(filename, type);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texObj);
	}

	void Texture::createTexture(std::string filename, int type)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &texObj);
		glBindTexture(GL_TEXTURE_2D, texObj);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		imageData = nullptr;
		loadTexture(filename.data());
		if (imageData)
		{
			switch (nrChannels)
			{
			case 1:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, resX, resY, 0, GL_RED, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				imageFormat = R;

				break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, resX, resY, 0, GL_RG, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				imageFormat = RG;

				break;
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resX, resY, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				imageFormat = RGB;

				break;
			case 4:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resX, resY, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
				glGenerateMipmap(GL_TEXTURE_2D);
				imageFormat = RGBA;

				break;
			}

			
		}
		else
		{
			std::cout << "couldn't load image sorry bro" << std::endl;
			return;
		}

		unbind();
		stbi_image_free(imageData);
	}

	void Texture::useTexture(unsigned int texIndex)
	{
		glActiveTexture(GL_TEXTURE0 + texIndex);
		glBindTexture(GL_TEXTURE_2D, texObj);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::loadTexture(const char* filename)
	{
		imageData = stbi_load(filename, &resX, &resY, &nrChannels, 0);
	}

}
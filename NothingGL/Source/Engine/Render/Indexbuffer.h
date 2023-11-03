#pragma once
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Renderer
{

	class IndexBuffer
	{
	private:
		unsigned int IBO;
		unsigned int count;

	public:
		IndexBuffer();
		~IndexBuffer();

		void createBuffer(std::vector<unsigned int>& indices);
		void bind();

		const unsigned int& getCount() { return count; }
	};

}
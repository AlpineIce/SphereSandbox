#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <GLM/glm.hpp>
#include <vector>

namespace Renderer
{

	struct Vertex
	{
		glm::vec3 positions;
		glm::vec3 normals;
		glm::vec2 texCoords;
	};


	class VertexBuffer
	{
	private:
		unsigned int VBO;
		const void* data;

	public:
		VertexBuffer();
		~VertexBuffer();

		void createBuffer(const std::vector<Vertex>& vertices);
		void bind();

		inline unsigned int getVBO() { return VBO; }
		const inline void* getData() { return data; }
	};

}
#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <vector>

namespace Renderer
{

	struct Vertex
	{
		glm::vec3 positions;
		glm::vec3 normals;
		glm::vec2 texCoords;
		glm::vec3 tangents;
		glm::vec3 bitangents;
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
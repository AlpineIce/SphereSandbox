#include "Vertexbuffer.h"
#include <iostream>

namespace Renderer
{

	VertexBuffer::VertexBuffer()
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &VBO);
	}

	void VertexBuffer::createBuffer(const std::vector<Vertex>& vertices)
	{
		glCreateBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	}


	void VertexBuffer::bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}

}

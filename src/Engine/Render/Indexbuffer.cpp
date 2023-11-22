#include "Indexbuffer.h"

namespace Renderer
{

	IndexBuffer::IndexBuffer()
	{


	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &IBO);
	}

	void IndexBuffer::createBuffer(std::vector<unsigned int>& indices)
	{
		count = (unsigned int)indices.size();

		glCreateBuffers(1, &IBO);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	}

	void IndexBuffer::bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	}

}
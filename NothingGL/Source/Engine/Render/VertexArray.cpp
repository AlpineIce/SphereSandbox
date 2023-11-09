#include "VertexArray.h"
#include <iostream>

namespace Renderer
{

	VertexArray::VertexArray()
	{
	
	
		VBindex = 0;
		ib = NULL;
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &VAO);
	}

	void VertexArray::createBuffer()
	{
		glCreateVertexArrays(1, &VAO);
	}

	void VertexArray::addBuffer(VertexBuffer& vb)
	{
		bind();
		vb.bind();

		//vertex positions
		glEnableVertexAttribArray(VBindex);
		glVertexAttribPointer(VBindex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		VBindex++;

		//vertex normals
		glEnableVertexAttribArray(VBindex);
		glVertexAttribPointer(VBindex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normals));
		VBindex++;

		//texture coordinates
		glEnableVertexAttribArray(VBindex);
		glVertexAttribPointer(VBindex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		VBindex++;

		//tangents
		glEnableVertexAttribArray(VBindex);
		glVertexAttribPointer(VBindex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangents));
		VBindex++;

		//bitangents
		glEnableVertexAttribArray(VBindex);
		glVertexAttribPointer(VBindex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangents));
		VBindex++;
	
		vertexBuffers.push_back(vb);
	}

	void VertexArray::setIndexBuffer(IndexBuffer& ib)
	{
		this->ib = &ib;

		bind();
		ib.bind();
	}

	void VertexArray::bind()
	{
		glBindVertexArray(VAO);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

}
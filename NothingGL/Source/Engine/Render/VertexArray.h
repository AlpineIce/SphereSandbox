#pragma once
#include "Vertexbuffer.h"
#include "Indexbuffer.h"
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Renderer
{

	class VertexArray
	{
	private:
		unsigned int VAO;
		unsigned int VBindex;
		IndexBuffer* ib;

		std::vector<VertexBuffer> vertexBuffers;

	public:
		VertexArray();
		~VertexArray();

		void createBuffer();
		void addBuffer(VertexBuffer& vertexbuffer);
		void setIndexBuffer(IndexBuffer& ib);
		void bind() const;
		void unbind() const;

		const unsigned int& getVAO() { return VAO; }
	};

}
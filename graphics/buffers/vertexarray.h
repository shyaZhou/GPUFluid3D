#pragma once

#include <vector>
#include <gl/glew.h>
#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexbuffer.h"
namespace sparky
{
	namespace graphics {

		class VertexArray
		{
		private:
			GLuint m_ArrayID;
			std::vector<Buffer*> m_Buffer;
			IndexBuffer* m_IndexBuffer;
			bool HasIndex;

		public:
			VertexArray();
			~VertexArray();
			void addBuffer(Buffer* buffer, GLuint index);
			void setIndexBuffer(IndexBuffer* ibuffer);
			void bind();
			void unBind();
			unsigned int Indexcount();
			unsigned int Vertexcout();
			bool Indexed();
		};
	}
}
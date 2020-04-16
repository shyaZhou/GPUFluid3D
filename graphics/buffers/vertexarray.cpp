#include "vertexarray.h"

namespace sparky {
	namespace graphics
	{
		VertexArray::VertexArray()
		{
			m_IndexBuffer = 0;
			glGenVertexArrays(1, &m_ArrayID);
		}

		VertexArray::~VertexArray()
		{
		/*	for (int i = 0; i < m_Buffer.size(); i++)
			{
				delete m_Buffer[i];
			}*/
		}

		void VertexArray::addBuffer(Buffer* buffer, GLuint index)
		{
			bind();
			buffer->bind();
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, buffer->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);
			
			unBind();
			glDisableVertexAttribArray(index);
			buffer->unBind();
			m_Buffer.push_back(buffer);
		}

		void VertexArray::setIndexBuffer(IndexBuffer* ibuffer)
		{
			bind();
			m_IndexBuffer = ibuffer;
			ibuffer->bind();
			 
			
			unBind();
			ibuffer->unBind();
		}

		void VertexArray::bind()
		{
			glBindVertexArray(m_ArrayID);
		}

		void VertexArray::unBind()
		{
			glBindVertexArray(0);
		}

		bool VertexArray::Indexed()
		{
			if (m_IndexBuffer) return true;
			return false;
		}

		unsigned int VertexArray::Vertexcout()
		{
			return 0;
		}

		unsigned int VertexArray::Indexcount()
		{
			if (m_IndexBuffer)
			{
				return m_IndexBuffer->count();
			}
			return 0;
		}
	}
}
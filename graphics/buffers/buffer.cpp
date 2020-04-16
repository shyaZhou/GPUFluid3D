#include "graphics/buffers/buffer.h"


namespace sparky {
	namespace graphics {

		Buffer::Buffer(GLfloat* data, GLsizei count, GLuint componentCount)
		{
			m_ComponentCount = componentCount;
			glGenBuffers(1, &m_BufferID);
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(GLfloat), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
		}

		void Buffer::bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
		}

		void Buffer::unBind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		GLuint Buffer::GetComponentCount() const
		{
			return m_ComponentCount;
		}
	}
}
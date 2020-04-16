#include "computebuffer.h"

namespace sparky {
	namespace graphics {

		ComputeBuffer::ComputeBuffer( GLuint structCount, GLsizei structsize, void* data)
		{
			m_StructCount = structCount;
			m_StructSize = structsize;
			glGenBuffers(1, &m_BufferID);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, structCount * structsize, data, GL_STATIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		}

		void ComputeBuffer::bind(GLuint bindpoint) const
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindpoint, m_BufferID);
		}

		//void ComputeBuffer::unBind() const
		//{
		//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		//}
		//GLuint ComputeBuffer::GetBindPoint() const
		//{
		//	//glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "shader_data");
		//}
 
	}
}

//https://blog.csdn.net/koibiki/article/details/80590885
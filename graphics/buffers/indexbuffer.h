#pragma once

#include <gl/glew.h>
namespace sparky
{
	namespace graphics
	{
		class IndexBuffer
		{
		private:
			GLuint m_BufferID;
			GLuint m_Count;
		public:
			IndexBuffer(GLushort* data, GLsizei count);

			void bind() const;
			void unBind() const;
			unsigned int count() const;

		};
	}
}

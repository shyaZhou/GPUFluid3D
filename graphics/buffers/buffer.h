#pragma once

#include <gl/glew.h>
namespace sparky
{
	namespace graphics
	{
		class Buffer
		{
		private:
			GLuint m_BufferID;
			GLuint m_ComponentCount;
		public:
			Buffer(GLfloat* data, GLsizei count, GLuint componentCount);

			GLuint GetComponentCount() const;
			void bind() const;
			void unBind() const;

		};
	}
}

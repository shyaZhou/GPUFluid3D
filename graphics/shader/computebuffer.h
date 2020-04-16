#pragma once
#include<gl/glew.h>

namespace sparky {
	namespace graphics {

		class ComputeBuffer
		{
		private:
			GLuint m_BufferID;
			GLuint m_StructCount;
			GLsizei m_StructSize;
		public:
			ComputeBuffer(GLuint StructCount, GLsizei StructSize, void* data = 0);
			//GLuint GetBindPoint() const;
			//GLuint GetComponentCount() const;
			void bind(GLuint bindpoint) const;
			//void unBind() const;
	 
		};

	}
}
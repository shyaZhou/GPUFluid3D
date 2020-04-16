#pragma once
#pragma once
#include "shader.h"
namespace sparky {
	namespace graphics {
		class ComputeShader :public Shader
		{
		public:
			void Dispatch(int groupx, int groupy, int groupz);
			ComputeShader(const char* cmpPath);
		private:
			GLuint load();
			const char*m_CmpPath;
		};
	}
}
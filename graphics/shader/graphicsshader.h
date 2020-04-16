#pragma once
#include "shader.h"
namespace sparky {
	namespace graphics {
		class GraphicsShader :public Shader
		{
		public:
			GraphicsShader(const char* vertPath, const char* fragPath);
		private:
			GLuint load();
			const char*m_VertPath, *m_FragPath;
		};
	}
}
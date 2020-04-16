#pragma once

#include <gl/glew.h>
//#include "maths/mat4.h"
//#include "maths/vec2.h"
//#include "maths/vec3.h"
//#include "maths/vec4.h"
#include "computebuffer.h"
#include "glm/gtc/matrix_transform.hpp"
//using namespace sparky::maths;
namespace sparky {
	namespace graphics {
		class Shader
		{
		protected:
			GLuint m_ShaderID;
		private:
			const char*m_VertPath, *m_FragPath;
		
			
		public:
			//Shader(const char* vertPath, const char* fragPath);
			~Shader();
			void enable();
			void disable();

			GLint GetUniformLocation(const GLchar* name);
			void setUniformMat41(const GLchar* name, const glm::mat4& matrix);
			//void setUniformMat4(const GLchar* name, const mat4& matrix);
			void setUniform1f(const GLchar* name, float value);
			void setUniform1i(const GLchar* name, int value);
			//void setUniform2f(const GLchar* name, const vec2& vector);
			void setUniform2f(const GLchar* name, const glm::vec2& vector);
			//void setUniform3f(const GLchar* name, const vec3& vector);
			void setUniform3f(const GLchar* name, const glm::vec3& vector);
			//void setUniform4f(const GLchar* name, const vec4& vector);
			void setUniform4f(const GLchar* name, const glm::vec4& vector);
			void setBuffer(const GLchar* name, const ComputeBuffer& cb);
			//void setUniform4f(const GLchar* name, const vec4& vector);

		};
	}
}
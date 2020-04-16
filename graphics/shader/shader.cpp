#include "shader.h"
#include <assert.h>

#include "glm/gtc/type_ptr.hpp"
namespace sparky {
	namespace graphics {

		Shader::~Shader()
		{
			glDeleteShader(m_ShaderID);
		}

		void Shader::enable() {
			glUseProgram(m_ShaderID);
		}

		void Shader::disable()
		{
			glUseProgram(0);
		}

		void Shader::setUniformMat41(const GLchar* name, const glm::mat4& matrix)
		{
			int t = GetUniformLocation(name);
			glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}

		//void Shader::setUniformMat4(const GLchar* name, const mat4& matrix)
		//{
		//	int t = GetUniformLocation(name);
		//	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix.elements);
		//	GLenum  Error = glGetError();
		//	assert(Error == GL_NO_ERROR);
		//}

		GLint Shader::GetUniformLocation(const GLchar* name)
		{
			return glGetUniformLocation(m_ShaderID, name);
		}

		void Shader::setUniform1f(const GLchar* name, float value)
		{
			glUniform1f(GetUniformLocation(name), value);
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}
		void Shader::setUniform1i(const GLchar* name, int value)
		{
			glUniform1i(GetUniformLocation(name), value);
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}
		//void Shader::setUniform2f(const GLchar* name, const vec2& vector)
		//{
		//	glUniform2f(GetUniformLocation(name), vector.x, vector.y);
		//	GLenum  Error = glGetError();
		//	assert(Error == GL_NO_ERROR);
		//}

		void Shader::setUniform2f(const GLchar* name, const glm::vec2& vector)
		{
			glUniform2f(GetUniformLocation(name), vector.x, vector.y);
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}

		//void Shader::setUniform3f(const GLchar* name, const vec3& vector)
		//{
		//	GLint  Location = GetUniformLocation(name);
		//	glUniform3f(Location, vector.x, vector.y, vector.z);
		//	GLenum  Error = glGetError();
		//	assert(Error == GL_NO_ERROR);

		//}

		void Shader::setUniform3f(const GLchar* name, const glm::vec3& vector)
		{
			GLint  Location = GetUniformLocation(name);
			glUniform3f(Location, vector.x, vector.y, vector.z);
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}

		//void Shader::setUniform4f(const GLchar* name, const vec4& vector)
		//{
		//	GLint  Location = GetUniformLocation(name);
		//	glUniform4f(Location, vector.x, vector.y, vector.z, vector.w);

		//	GLenum  Error = glGetError();
		//	assert(Error == GL_NO_ERROR);
		//}


		void Shader::setUniform4f(const GLchar* name, const glm::vec4& vector)
		{
			GLint  Location = GetUniformLocation(name);
			glUniform4f(Location, vector.x, vector.y, vector.z, vector.w);

			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
		}

		//http://www.itkeyword.com/doc/943193461334959x368/how-to-get-a-binding-point-of-an-image-variable-in-opengles
		void Shader::setBuffer(const GLchar* name, const ComputeBuffer& cb)
		{
			GLuint block_index = glGetProgramResourceIndex(m_ShaderID, GL_SHADER_STORAGE_BLOCK, name);

			GLint ValuesWritten = 0;
			GLint Binding = -1;
			GLenum  Prop = GL_BUFFER_BINDING;
			glGetProgramResourceiv(m_ShaderID, GL_SHADER_STORAGE_BLOCK, block_index, 1, &Prop, 1, &ValuesWritten, &Binding);

			glShaderStorageBlockBinding(m_ShaderID, block_index, Binding);
			cb.bind(Binding);
			GLenum  Error = glGetError();
			assert(Error == GL_NO_ERROR);
			 
		}
	}
}

//https://blog.csdn.net/zhouchengyunew/article/details/52769767
//https://blog.csdn.net/freehyan/article/details/52078011
//https://blog.csdn.net/freehyan/article/details/52078011?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task
//https://blog.csdn.net/z444_579/article/details/55506559?locationNum=9&fps=1

//http://www.geeks3d.com/20140704/tutorial-introduction-to-opengl-4-3-shader-storage-buffers-objects-ssbo-demo/
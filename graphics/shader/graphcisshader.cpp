#include "graphics/shader/graphicsshader.h"
#include <gl/glew.h>
#include <vector>
#include <iostream>
#include "utils/fileutils.h"
namespace sparky {
	namespace graphics {

		GraphicsShader::GraphicsShader(const char* vertPath, const char* fragPath) :m_VertPath(vertPath), m_FragPath(fragPath)
		{
			m_ShaderID = load();
		}

		GLuint GraphicsShader::load()
		{
			GLuint program = glCreateProgram();
			GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
			GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

			std::string vsource = FileUtile::read_file(m_VertPath);
			std::string fsource = FileUtile::read_file(m_FragPath);
			const char* vertSource = vsource.c_str();
			const char* fragSource = fsource.c_str();

			//std::cout << FileUtile::read_file(m_VertPath) << std::endl;

			glShaderSource(vertex, 1, &vertSource, NULL);
			glCompileShader(vertex);

			GLint result;
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(vertex, length, &length, &error[0]);

				std::cout << "Vertex Shader Compile Failed" << &error[0] << std::endl;
				glDeleteShader(vertex);
				return  0;
			}


			glShaderSource(fragment, 1, &fragSource, NULL);
			glCompileShader(fragment);


			glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(fragment, length, &length, &error[0]);

				std::cout << "Fragment Shader Compile Failed" << &error[0] << std::endl;
				glDeleteShader(fragment);
				return  0;
			}
			//GL_INVALID_OPERATION
			glAttachShader(program, vertex);
			glAttachShader(program, fragment);
			glLinkProgram(program);
			glValidateProgram(program);
		
			GLint t;
			glGetProgramiv(program, GL_VALIDATE_STATUS, &t);
			if (t == GL_FALSE)
			{
				int infologLength = 0;
				int charsWritten = 0;
				char *infoLog;

				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);


				GLsizei length;
				GLchar info[1000];
				glGetProgramInfoLog(program, 1000, &length, info);
				std::cout << "Link Error" << info << std::endl;
			}
			glDeleteShader(vertex);
			glDeleteShader(fragment);

			return program;
		}
	}
}
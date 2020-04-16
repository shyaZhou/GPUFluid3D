#include "graphics/shader/computeshader.h"
#include <gl/glew.h>
#include <vector>
#include <iostream>
#include "utils/fileutils.h"
#include <assert.h>
namespace sparky {
	namespace graphics {

		ComputeShader::ComputeShader(const char* cmpPath) :m_CmpPath(cmpPath)
		{
			m_ShaderID = load();
		}

		GLuint ComputeShader::load()
		{
			GLuint program = glCreateProgram();
			GLuint compute = glCreateShader(GL_COMPUTE_SHADER);
		 

			std::string cs = FileUtile::read_file(m_CmpPath);
			const char* cmpSource = cs.c_str();
			 

			glShaderSource(compute, 1, &cmpSource, NULL);
			glCompileShader(compute);

			

			GLint result;
			glGetShaderiv(compute, GL_COMPILE_STATUS, &result);
			if (result == GL_FALSE)
			{
				GLint length;
				glGetShaderiv(compute, GL_INFO_LOG_LENGTH, &length);
				std::vector<char> error(length);
				glGetShaderInfoLog(compute, length, &length, &error[0]);

				std::cout << "Shader Compile Failed: " << m_CmpPath << ":  " << &error[0] << std::endl;
				glDeleteShader(compute);
				assert(false);
				return  0;
			}


			glAttachShader(program, compute);
			 
			glLinkProgram(program);

			

			glValidateProgram(program);

			GLint t;
			glGetProgramiv(program, GL_LINK_STATUS, &t);
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
				assert(false);
				return  0;
			}

		
			glDeleteShader(compute);

			return program;
		}

		void ComputeShader::Dispatch(int groupx, int groupy, int groupz)
		{
			
			glDispatchCompute(groupx, groupy, groupz);
			
		}
	}
}
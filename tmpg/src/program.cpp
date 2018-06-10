#include "program.h"

namespace gl {

	void Program::CreateShader(GLenum type, const std::string& dir)
	{
		m_shaders.emplace_back(type);
		m_shaders[m_shaders.size() - 1].Compile(dir);
	}

	void Program::AttachShaders(void)
	{
		for (auto& shader : m_shaders) glAttachShader(m_programID, shader.ShaderID());
	}

	void Program::DeleteShaders(void)
	{
		for (auto& shader : m_shaders)
		{
			for (auto& shader : m_shaders) glDeleteShader(shader.ShaderID());
		}
	}

	bool Program::Status(void)
	{
		int32_t status;
		glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
		if (status != GL_TRUE)
		{
			std::cerr << "failed to create program\n";
			int32_t infoLogLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* buffer = (char*)alloca(infoLogLength * sizeof(char));
			GLsizei bufferSize;
			glGetProgramInfoLog(m_programID, infoLogLength * sizeof(char), &bufferSize, buffer);
			std::cerr << buffer << std::endl;
			return false;
		}
		return true;
	}

	void Program::UseProgram(void)
	{
		glUseProgram(m_programID);
	}

	void Program::Uniform1f(float p, uint32_t index)
	{
		glUniform1f(m_uniformLocations[index], p);
	}
	
	void Program::Uniform2f(float* p, uint32_t index)
	{
		glUniform2fv(1, m_uniformLocations[index], p);
	}

	void Program::Uniform3f(float* p, uint32_t index)
	{
		glUniform3fv(m_uniformLocations[index], 1, p);
	}

	void Program::UniformMat4(float* p, uint32_t index)
	{
		glUniformMatrix4fv(m_uniformLocations[index], 1, GL_FALSE, p);
	}

}
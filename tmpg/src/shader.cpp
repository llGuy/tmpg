#include "shader.h"

namespace gl {

	Shader::Shader(GLenum shadertype)
		: m_shadertype(shadertype)
	{
	}

	void Shader::Compile(const std::string& directory)
	{
		m_shaderID = glCreateShader(m_shadertype);
		auto src = ExtractSource(directory);
		if (src.has_value())
		{
			std::array<const char*, 1> srcs{ src.value().c_str() };
			glShaderSource(m_shaderID, 1, srcs.data(), 0);
			glCompileShader(m_shaderID);
		}
	}

	uint32_t Shader::ShaderID(void)
	{
		return m_shaderID;
	}

	GLenum Shader::Type(void) const
	{
		return m_shadertype;
	}

	std::optional<std::string> Shader::ExtractSource(const std::string& directory)
	{
		std::ifstream file(directory);
		if (!file.good())
		{
			std::cerr << "unable to extract from : " << directory << '\n';
			return std::optional<std::string>();
		}
		else return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	}

	void Shader::Delete(void)
	{
		glDeleteShader(m_shaderID);
	}

	bool Shader::Status(void)
	{
		int32_t status;
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE)
		{
			std::cerr << "failed to create shader : ";
			switch (m_shadertype)
			{
			case GL_VERTEX_SHADER: std::cerr << "vertex shader" << std::endl; break;
			case GL_FRAGMENT_SHADER: std::cerr << "fragment shader" << std::endl; break;
			case GL_GEOMETRY_SHADER: std::cerr << "geometry shader" << std::endl; break;
			}

			int32_t infoLogLength = 0;
			glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* buffer = (char*)alloca(infoLogLength * sizeof(char));
			GLsizei bufferSize;
			glGetShaderInfoLog(m_shaderID, infoLogLength * sizeof(char), &bufferSize, buffer);

			std::cerr << buffer << std::endl;
			return false;
		}
		return true;
	}

}	
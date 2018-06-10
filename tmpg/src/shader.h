#ifndef _SHADER_H_
#define _SHADER_H_

#include <GL/glew.h>
#include <optional>
#include <iostream>
#include <fstream>
#include <string>
#include <array>

namespace gl {

	class Shader
	{
	public:
		Shader(void) = default;
		Shader(GLenum shadertype);
		~Shader(void) = default;

		void Compile(const std::string& directory);
		bool Status(void);
		void Delete(void);
	private:
		void AttachShaders(void);
	public:
		uint32_t ShaderID(void);
		GLenum Type(void) const;
	private:
		std::optional<std::string> ExtractSource(const std::string& directory);
	private:
		uint32_t m_shaderID;
		GLenum m_shadertype;
	};

}	

#endif
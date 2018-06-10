#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <GL/glew.h>
#include <array>
#include <iostream>
#include <algorithm>
#include <functional>

#include "shader.h"

namespace gl {
	
	class Program
	{
	public:
		Program(void) = default;

		void UseProgram(void);
		void CreateShader(GLenum type, const std::string& dir);

		void Uniform1f(float p, uint32_t index);
		void Uniform2f(float* p, uint32_t index);
		void Uniform3f(float* p, uint32_t index);
		void UniformMat4(float* p, uint32_t index);
	public:
		template<typename... _Ty>
		void LinkShaders(_Ty... attribs)
		{
			bool success = true;
			for (auto& shader : m_shaders) success &= shader.Status();
			if (success)
			{
				m_programID = glCreateProgram();
				AttachShaders();
				BindAttribs(attribs...);
				glLinkProgram(m_programID);
				DeleteShaders();
			}
			// check program status
			Status();
		}
		template<typename... _Ty>
		void GetUniformLocations(_Ty... locs)
		{
			std::array<const char*, sizeof...(locs)> names { locs... };
			std::for_each(names.begin(), names.end(), [&](const char* name)
			{
				uint32_t location = glGetUniformLocation(m_programID, name);
				m_uniformLocations.push_back(location);
			});
		}
	private:
		bool Status(void);
		void AttachShaders();
		void DeleteShaders(void);

		template<typename... _Ty>
		void BindAttribs(_Ty... attribs)
		{
			std::array<const char*, sizeof...(attribs)> atts{ attribs... };
			for (uint32_t i = 0; i < sizeof...(attribs); ++i) glBindAttribLocation(m_programID, i, atts[i]);
		}
	public:
		static std::string ShaderPath(const std::string& sub)
		{
#ifdef _WIN32
			return "res/" + sub;
#else
			return "../res/" + sub;
#endif
		}
	private:
		std::vector<uint32_t> m_uniformLocations;
		std::vector<Shader> m_shaders;
		uint32_t m_programID;
	};

	/*template<uint32_t _Size, uint32_t _Locs>
	class Program
	{
	public:
		template<typename... _Sh>
		Program(_Sh... types)
			: m_shaders{ Shader(types)... }
		{
		}

		template<typename... _Dir>
		void Compile(_Dir&&... directories)
		{
			std::array<std::string, sizeof...(directories)> dirs{ std::forward<_Dir>(directories)... };
			for (uint32_t i = 0; i < _Size; ++i)
			{
				std::cout << "compiling : " << dirs[i] << std::endl;
				m_shaders[i].Compile(dirs[i]);
			}
		}
		template<typename... _Attribs>
		void Link(_Attribs... attributes)
		{
			bool success = true;
			for (auto& shader : m_shaders) success &= this->CheckShaderStatus(shader.ShaderID(), shader.Type());
			if (success)
			{
				m_programID = glCreateProgram();
				AttachShaders();
				BindAttribLocations(attributes...);
				glLinkProgram(m_programID);
				DeleteShaders();
			}
			this->CheckProgramStatus();
		}
		void UseProgram(void)
		{
			glUseProgram(m_programID);
		}
	private:
		// uniform data functions
		void Uniform2f(float* p, uint32_t l) { glUniform2fv(l, 1, p); }
		void Uniform3f(float* p, uint32_t l) { glUniform3fv(l, 1, p); }
		void UniformMat4(float* p, uint32_t l) { glUniformMatrix4fv(l, 1, GL_FALSE, p); }
		void Uniform1f(float* p, uint32_t l) { glUniform1f(l, *p); }

		uint32_t GetUniformLocation(const char* name) const
		{
			return glGetUniformLocation(m_programID, name);
		}
	public:
		template<typename... _L>
		void GetUniformLocations(_L&&... locs)
		{
			std::array<UDataLoc, sizeof...(locs)> locations{ locs... };
			for (uint32_t i = 0; i < sizeof...(locs); ++i)
			{
				m_udataLocations[i] = locations[i];
				m_udataLocations[i].location = GetUniformLocation(m_udataLocations[i].name);
			}
		}

		template<typename... _Ptr>
		void UniformData(_Ptr&&... ptrs)
		{
			using UDFunc = void(Program<_Size, _Locs>::*)(float*, uint32_t);

			static std::array<UDFunc, udata_t::INV> ufuncs
			{
				&Program<_Size, _Locs>::Uniform3f,
				&Program<_Size, _Locs>::Uniform2f,
				&Program<_Size, _Locs>::UniformMat4,
				&Program<_Size, _Locs>::Uniform1f
			};

			std::array<float*, sizeof...(ptrs)> ptrarr{ ptrs... };
			for (uint32_t i = 0; i < _Locs; ++i)
				(*this.*ufuncs[m_udataLocations[i].type])(ptrarr[i], m_udataLocations[i].location);
		}
	private:
		// checking status
		bool CheckShaderStatus(uint32_t shaderID, GLenum type)
		{
			int32_t status;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
			if (status != GL_TRUE)
			{
				std::cerr << "failed to create shader : ";
				switch (type)
				{
				case GL_VERTEX_SHADER: std::cerr << "vertex shader" << std::endl; break;
				case GL_FRAGMENT_SHADER: std::cerr << "fragment shader" << std::endl; break;
				case GL_GEOMETRY_SHADER: std::cerr << "geometry shader" << std::endl; break;
				}

				int32_t infoLogLength = 0;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
				char* buffer = (char*)alloca(infoLogLength * sizeof(char));
				GLsizei bufferSize;
				glGetShaderInfoLog(shaderID, infoLogLength * sizeof(char), &bufferSize, buffer);
				std::cerr << buffer << std::endl;
				return false;
			}
			return true;
		}
		bool CheckProgramStatus(void)
		{
			int32_t status;
			glGetProgramiv(m_programID, GL_LINK_STATUS, &status);
			if (status != GL_TRUE)
			{
				std::cerr << "failed to create shader\n";
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
	private:
		template<typename... _Attribs>
		void BindAttribLocations(_Attribs... attribs)
		{
			std::array<const char*, sizeof...(attribs)> atts{ attribs... };
			for (uint32_t i = 0; i < sizeof...(attribs); ++i) glBindAttribLocation(m_programID, i, atts[i]);
		}

		void DeleteShaders(void)
		{
			for (auto& shader : m_shaders) glDeleteShader(shader.ShaderID());
		}
		void AttachShaders(void)
		{
			for (auto& shader : m_shaders) glAttachShader(m_programID, shader.ShaderID());
		}
	public:
		// depends if user is on linux or windows
		static std::string ShaderPath(const std::string& sub)
		{
#ifdef _WIN32
			return "res/" + sub;
#else
			return "../res/" + sub;
#endif
		}
	private:
		std::array<Shader, _Size> m_shaders;
		std::array<UDataLoc, _Locs> m_udataLocations;
		uint32_t m_programID;
	};*/


}

#endif
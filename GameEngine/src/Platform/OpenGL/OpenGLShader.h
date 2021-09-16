#pragma once

#include "GameEngine/Renderer/Shader.h"

#include "glm/glm.hpp"

#include <unordered_map>

namespace GameEngine
{

	/**
	* Stores the source code for a shader after being parsed (see Shader::ParseShader).
	*/
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	/**
	* OpenGL Shader API.
	*/
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1i(const std::string& name, int v) override;
		virtual void SetUniform1f(const std::string& name, float v) override;
		virtual void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) override;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) override;
		virtual int GetUniformLocation(const std::string& name) override;

	private:
		struct ShaderProgramSource ParseShader(const std::string& filepath);
		int CompileShader(unsigned int type, const std::string& source);

		unsigned int m_id;
		std::string m_filepath;
		std::unordered_map<std::string, int> m_uniformLocationCache;
	};

}

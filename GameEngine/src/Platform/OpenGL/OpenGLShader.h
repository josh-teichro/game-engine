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
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1i(const std::string& name, int value) override;

		virtual void SetUniform1f(const std::string& name, float value) override;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& value) override;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& value) override;

		virtual void SetUniformMat3f(const std::string& name, const glm::mat3& value) override;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) override;

	private:
		int GetUniformLocation(const std::string& name);

		struct ShaderProgramSource ParseShader(const std::string& filepath);
		int CompileShader(unsigned int type, const std::string& source);

	private:
		unsigned int m_id;
		std::string m_filepath;
		std::unordered_map<std::string, int> m_uniformLocationCache;
	};

}

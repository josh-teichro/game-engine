#pragma once

#include "GameEngine/Renderer/Shader.h"

#include <glm/glm.hpp>

#include <unordered_map>
#include <glad/glad.h>

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
		OpenGLShader(const std::string& name, const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1i(const std::string& name, int value) override;

		virtual void SetUniform1iv(const std::string& name, int* value, uint32_t count) override;

		virtual void SetUniform1f(const std::string& name, float value) override;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& value) override;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& value) override;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& value) override;

		virtual void SetUniform1fv(const std::string& name, const float* value, uint32_t count) override;
		virtual void SetUniform2fv(const std::string& name, const glm::vec2* value, uint32_t count) override;
		virtual void SetUniform3fv(const std::string& name, const glm::vec3* value, uint32_t count) override;
		virtual void SetUniform4fv(const std::string& name, const glm::vec4* value, uint32_t count) override;

		virtual void SetUniformMat3f(const std::string& name, const glm::mat3& value) override;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) override;

		virtual void SetUniformMat3fv(const std::string& name, const glm::mat3* value, uint32_t count) override;
		virtual void SetUniformMat4fv(const std::string& name, const glm::mat4* value, uint32_t count) override;

		virtual const std::string& GetName() const override { return m_name; }

	private:
		int GetUniformLocation(const std::string& name);

		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> ParseShader(const std::string& shaderSource);
		int CompileShader(std::unordered_map<GLenum, std::string> shaderSources);

	private:
		unsigned int m_id;
		std::string m_name;
		std::unordered_map<std::string, int> m_uniformLocationCache;
	};

}

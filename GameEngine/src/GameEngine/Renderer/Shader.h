#pragma once

namespace GameEngine
{
	/**
	* Shader API.
	*/
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(const std::string& name, int value) = 0;

		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetUniformMat3f(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;

		static std::shared_ptr<Shader> Create(const std::string& filepath);

	};

}


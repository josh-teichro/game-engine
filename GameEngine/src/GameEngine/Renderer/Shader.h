#pragma once

namespace GameEngine
{
	/**
	* Shader API.
	*/
	class Shader
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1i(const std::string& name, int v) = 0;
		virtual void SetUniform1f(const std::string& name, float v) = 0;
		virtual void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) = 0;
		virtual int GetUniformLocation(const std::string& name) = 0;

		static std::shared_ptr<Shader> Create(const std::string& filepath);

	};

}


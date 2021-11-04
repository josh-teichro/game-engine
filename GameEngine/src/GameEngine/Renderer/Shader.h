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

		virtual void SetUniform1iv(const std::string& name, int* value, uint32_t count) = 0;

		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniform2f(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetUniform3f(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetUniform4f(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetUniform1fv(const std::string& name, const float* value, uint32_t count) = 0;
		virtual void SetUniform2fv(const std::string& name, const glm::vec2* value, uint32_t count) = 0;
		virtual void SetUniform3fv(const std::string& name, const glm::vec3* value, uint32_t count) = 0;
		virtual void SetUniform4fv(const std::string& name, const glm::vec4* value, uint32_t count) = 0;

		virtual void SetUniformMat3f(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;

		virtual void SetUniformMat3fv(const std::string& name, const glm::mat3* value, uint32_t count) = 0;
		virtual void SetUniformMat4fv(const std::string& name, const glm::mat4* value, uint32_t count) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

	};

	/**
	* Shader Library.
	*/
	class ShaderLibrary
	{
	public:
		void Add(Ref<Shader> shader);
		void Add(const std::string& name, Ref<Shader> shader);

		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};

}


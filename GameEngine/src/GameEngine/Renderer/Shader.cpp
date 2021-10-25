#include "gepch.h"

#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GameEngine 
{

	// -----------------------------------------------------------------
	// Shader ----------------------------------------------------------
	// -----------------------------------------------------------------

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLShader>(filepath);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::OpenGL: return MakeRef<OpenGLShader>(name, filepath);
		default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

	// -----------------------------------------------------------------
	// ShaderLibrary ---------------------------------------------------
	// -----------------------------------------------------------------

	void ShaderLibrary::Add(Ref<Shader> shader)
	{
		Add(shader->GetName(), shader);
	}

	void ShaderLibrary::Add(const std::string& name, Ref<Shader> shader)
	{
		GE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		GE_CORE_ASSERT(Exists(name), "Shader doesn't exist!");
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_shaders.find(name) != m_shaders.end();
	}

}

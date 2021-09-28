#include "gepch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GameEngine 
{

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLShader>(filepath);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLShader>(vertexSrc, fragmentSrc);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

}

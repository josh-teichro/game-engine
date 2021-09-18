#include "gepch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GameEngine 
{

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case Renderer::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

}

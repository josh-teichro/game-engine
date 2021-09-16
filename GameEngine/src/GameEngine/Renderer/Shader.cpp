#include "gepch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GameEngine 
{

	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLShader(filepath);
		default: GE_CORE_ASSERT(false, "Render API currently does not support Shader!"); return nullptr;
		}
	}

}

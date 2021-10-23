#include "gepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace GameEngine
{

	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (RenderCommand::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLTexture2D>(filepath);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Texture2D!"); return nullptr;
		}
	}

}

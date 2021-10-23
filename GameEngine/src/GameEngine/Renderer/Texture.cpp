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

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (RenderCommand::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLTexture2D>(width, height);
			default: GE_CORE_ASSERT(false, "Render API currently does not support Texture2D!"); return nullptr;
		}
	}

}

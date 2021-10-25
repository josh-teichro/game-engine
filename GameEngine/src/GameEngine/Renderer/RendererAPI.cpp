#include "gepch.h"

#include "GameEngine/Renderer/RendererAPI.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace GameEngine
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeScope<OpenGLRendererAPI>();
			default: GE_CORE_ASSERT(false, "Renderer API curently not supported!"); return nullptr;
		}
	}
}

#include "gepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GameEngine
{

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
			default: GE_CORE_ASSERT(false, "Render API currently does not support VertexArray!"); return nullptr;
		}
	}

}

#include "gepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GameEngine
{

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLVertexArray>();
			default: GE_CORE_ASSERT(false, "Render API currently does not support VertexArray!"); return nullptr;
		}
	}

}

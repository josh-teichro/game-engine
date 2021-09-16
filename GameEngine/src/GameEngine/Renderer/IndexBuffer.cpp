#include "gepch.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace GameEngine
{

	IndexBuffer* IndexBuffer::Create(const unsigned int* data, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLIndexBuffer(data, count);
		default: GE_CORE_ASSERT(false, "Render API currently does not support IndexBuffer!"); return nullptr;
		}
	}

}

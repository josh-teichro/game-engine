#include "gepch.h"

#include "GameEngine/Renderer/IndexBuffer.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace GameEngine
{

	Ref<IndexBuffer> IndexBuffer::Create(const unsigned int* data, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLIndexBuffer>(data, count);
			default: GE_CORE_ASSERT(false, "Render API currently does not support IndexBuffer!"); return nullptr;
		}
	}

}

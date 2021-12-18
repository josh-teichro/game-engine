#include "gepch.h"

#include "GameEngine/Renderer/FrameBuffer.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace GameEngine
{

	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferProps props)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLFrameBuffer>(props);
			default: GE_CORE_ASSERT(false, "Render API currently does not support FrameBuffers!"); return nullptr;
		}
	}

}

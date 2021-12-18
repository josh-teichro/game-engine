#pragma once

#include "GameEngine/Core/Core.h"

namespace GameEngine
{

	struct FrameBufferProps
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetWidth(uint32_t width) = 0;
		virtual void SetHeight(uint32_t height) = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		static Ref<FrameBuffer> Create(FrameBufferProps props);
	};

}

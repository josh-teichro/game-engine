#pragma once

#include "GameEngine/Renderer/FrameBuffer.h"

namespace GameEngine
{

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferProps props);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return m_colorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() const override { return m_depthAttachment; }
		virtual uint32_t GetWidth() const override { return m_props.width; }
		virtual uint32_t GetHeight() const override { return m_props.height; }

		virtual void SetWidth(uint32_t width) override { m_props.width = width; }
		virtual void SetHeight(uint32_t height) override { m_props.height = height; }

		void Invalidate();
		void Resize(uint32_t width, uint32_t height) override;

	private:
		FrameBufferProps m_props;
		uint32_t m_id;		
		uint32_t m_colorAttachment, m_depthAttachment;

	};

}

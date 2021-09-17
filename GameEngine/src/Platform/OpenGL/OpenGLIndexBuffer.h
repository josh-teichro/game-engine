#pragma once

#include "GameEngine/Renderer/IndexBuffer.h"

namespace GameEngine
{

	/**
	* OpenGL IndexBuffer API.
	*/
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const unsigned int* data, unsigned int count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline unsigned int GetCount() const override { return m_count; }

	private:
		uint32_t m_id;
		uint32_t m_count;
	};

}

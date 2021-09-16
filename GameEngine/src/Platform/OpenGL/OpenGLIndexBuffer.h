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
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual inline unsigned int GetCount() const override { return m_count; }

	private:
		unsigned int m_id;
		unsigned int m_count;
	};

}

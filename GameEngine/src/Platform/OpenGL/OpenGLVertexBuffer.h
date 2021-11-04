#pragma once

#include "GameEngine/Renderer/VertexBuffer.h"

namespace GameEngine
{
	/**
	* Convert shader type to OpenGL base type.
	*/
	uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

	/**
	* OpenGL VertexBuffer API.
	*/
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size, const VertexBufferLayout& layout);
		OpenGLVertexBuffer(const void* data, uint32_t size, const VertexBufferLayout& layout);
		virtual  ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_layout = layout; }
		virtual void SetData(const void* data, uint32_t size) override;

	private:
		uint32_t m_id;
		VertexBufferLayout m_layout;
	};

}

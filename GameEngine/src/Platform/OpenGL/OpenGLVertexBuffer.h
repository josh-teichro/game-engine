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
		OpenGLVertexBuffer(const void* data, unsigned int size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		unsigned int m_id;
	};

}

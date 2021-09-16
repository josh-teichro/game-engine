#pragma once

#include "GameEngine/Renderer/VertexArray.h"

namespace GameEngine
{

	/**
	* OpenGL VertexArray API.
	*/
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		unsigned int m_id;
	};

}

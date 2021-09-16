#pragma once

#include "VertexBuffer.h"

namespace GameEngine
{

	class VertexArray
	{
	public:
		virtual void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexArray* Create();
	};

}

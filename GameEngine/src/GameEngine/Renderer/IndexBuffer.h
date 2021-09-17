#pragma once

namespace GameEngine
{

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline unsigned int GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(const unsigned int* data, unsigned int count);
	};

}

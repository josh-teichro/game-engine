#pragma once

namespace GameEngine
{
	enum class ShaderDataType
	{
		Float, Vec2, Vec3, Vec4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type);

	/**
	* Vertex buffer element.
	*/
	struct VertexBufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		uint32_t GetComponentCount() const;
	};


	/**
	* Vertex buffer layout.
	*/
	class VertexBufferLayout 
	{
	public:
		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);
		virtual ~VertexBufferLayout() = default;

		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }

		inline std::vector<VertexBufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<VertexBufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<VertexBufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<VertexBufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		uint32_t m_stride;
		std::vector<VertexBufferElement> m_elements;

		void CalculateOffsetsAndStride();
	};

	/**
	* Vertex buffer.
	*/
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const VertexBufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const VertexBufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(const void* data, unsigned int size, const VertexBufferLayout& layout);
	};

}

#include "gepch.h"

#include "GameEngine/Renderer/VertexBuffer.h"
#include "GameEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace GameEngine
{

	uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Vec2:     return 4 * 2;
			case ShaderDataType::Vec3:     return 4 * 3;
			case ShaderDataType::Vec4:     return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	// -----------------------------------------------------------------
	// VertexBufferElement ---------------------------------------------
	// -----------------------------------------------------------------

	VertexBufferElement::VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized) :
		name(name),
		type(type),
		offset(0),
		size(ShaderDataTypeSize(type)),
		normalized(normalized)
	{
	}

	uint32_t VertexBufferElement::GetComponentCount() const
	{
		switch (type)
		{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Vec2:    return 2;
			case ShaderDataType::Vec3:    return 3;
			case ShaderDataType::Vec4:    return 4;
			case ShaderDataType::Mat3:    return 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
		}

		GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	// -----------------------------------------------------------------
	// VertexBufferLayout ----------------------------------------------
	// -----------------------------------------------------------------

	VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements) :
		m_elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	void VertexBufferLayout::CalculateOffsetsAndStride()
	{
		GE_PROFILE_FUNCTION();

		uint32_t offset = 0;
		for (auto& el : m_elements)
		{
			el.offset = offset;
			offset += el.size;
		}
		m_stride = offset;
	}

	// -----------------------------------------------------------------
	// VertexBuffer ----------------------------------------------------
	// -----------------------------------------------------------------

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, unsigned int size, const VertexBufferLayout& layout)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::OpenGL: return MakeRef<OpenGLVertexBuffer>(data, size, layout);
			default: GE_CORE_ASSERT(false, "Render API currently does not support VertexBuffer!"); return nullptr;
		}
	}

}

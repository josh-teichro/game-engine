#include "gepch.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace GameEngine
{

    uint32_t ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Vec2:     return GL_FLOAT;
            case ShaderDataType::Vec3:     return GL_FLOAT;
            case ShaderDataType::Vec4:     return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        GE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    // -----------------------------------------------------------------
    // OpenGLVertexBuffer ----------------------------------------------
    // -----------------------------------------------------------------


    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, unsigned int size, const VertexBufferLayout& layout) :
        m_layout(layout)
    {
        GE_PROFILE_FUNCTION();

        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        GE_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_id);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        GE_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        GE_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

}

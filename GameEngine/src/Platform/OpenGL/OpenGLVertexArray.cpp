#include "gepch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace GameEngine
{

    OpenGLVertexArray::OpenGLVertexArray() :
        m_vertexBufferIndex(0)
    {
        GE_PROFILE_FUNCTION();

        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        GE_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_id);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        GE_PROFILE_FUNCTION();

        static const uint32_t maxComponentsPerAttrib = 4;

        GE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBufferLayout has no elements!");

        glBindVertexArray(m_id);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();

        for (const auto& element : layout)
        {
            if (element.GetComponentCount()[1] == 1) {
                glEnableVertexAttribArray(m_vertexBufferIndex);
                glVertexAttribPointer(
                    m_vertexBufferIndex,
                    element.GetComponentCount()[0],
                    ShaderDataTypeToOpenGLBaseType(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    (const void*)(uintptr_t)element.offset
                );
                m_vertexBufferIndex++;
            }
            else {
                // not tested (see https://github.com/TheCherno/Hazel/commit/6a93423cd9a1d81a605345afd1b432a34a44da49)
                int m = element.GetComponentCount()[0], n = element.GetComponentCount()[1];
                for (int i = 0; i < n; i++)
                {
                    glEnableVertexAttribArray(m_vertexBufferIndex);
                    glVertexAttribPointer(
                        m_vertexBufferIndex,
                        m,
                        ShaderDataTypeToOpenGLBaseType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        (const void*)(sizeof(float) * m * i)
                    );
                    glVertexAttribDivisor(m_vertexBufferIndex, 1);
                    m_vertexBufferIndex++;
                }
            }
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        GE_PROFILE_FUNCTION();

        indexBuffer->Bind();
        m_indexBuffer = indexBuffer;
    }


    void OpenGLVertexArray::Bind() const
    {
        GE_PROFILE_FUNCTION();

        glBindVertexArray(m_id);
    }

    void OpenGLVertexArray::Unbind() const
    {
        GE_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

}

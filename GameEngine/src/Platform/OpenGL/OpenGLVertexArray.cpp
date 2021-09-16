#include "gepch.h"
#include "OpenGLVertexArray.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace GameEngine
{

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glGenVertexArrays(1, &m_id);
        Bind();
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void OpenGLVertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
    {
        Bind();
        vb.Bind();

        const auto& elements = layout.GetElements();

        for (int i = 0; i < elements.size(); i++)
        {
            const auto& el = elements[i];

            glEnableVertexAttribArray(i); 
            glVertexAttribPointer(
                i, 
                el.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(el.type), 
                el.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(), 
                (const void*)el.offset
            );
        }
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_id);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

}

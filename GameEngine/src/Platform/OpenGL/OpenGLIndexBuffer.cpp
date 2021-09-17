#include "gepch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace GameEngine
{

    OpenGLIndexBuffer::OpenGLIndexBuffer(const unsigned int* data, unsigned int count) :
        m_count(count)
    {
        GE_CORE_ASSERT(sizeof(unsigned int) == sizeof(GLuint), "OpenGL index buffer data type does not match API.");

        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}

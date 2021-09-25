#include "gepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>

// TODO: Switch to newer OpenGL functions (replace code with comments)

namespace GameEngine
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) :
		m_filepath(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		GE_CORE_ASSERT(data, "Failed to load image!");
		m_width = width;
		m_height = height;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		//glTextureStorage2D(m_id, 1, GL_RGB8, m_width, m_height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

		//stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_id);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glBindTextureUnit(slot, m_id);
	}

}

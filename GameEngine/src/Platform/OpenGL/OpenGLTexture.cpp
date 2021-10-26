#include "gepch.h"

#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

// TODO: Switch to newer OpenGL functions (replace code with comments)

namespace GameEngine
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) :
		m_filepath(filepath),
		m_internalFormat(0),
		m_dataFormat(0)
	{
		GE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		GE_CORE_ASSERT(data, "Failed to load image!");
		m_width = width;
		m_height = height;

		if (channels == 4)
		{
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
		}

		GE_CORE_ASSERT(m_internalFormat & m_dataFormat, "Failed to load image: format not supported!");

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		//glTextureStorage2D(m_id, 1, internalFormat, m_width, m_height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		SetWrapMode(m_wrapMode);

		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, data);
		//glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		//stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_width(width), m_height(height)
	{
		GE_PROFILE_FUNCTION();

		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
		//glTextureStorage2D(m_id, 1, m_internalFormat, m_width, m_height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		SetWrapMode(m_wrapMode);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		GE_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_id);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		GE_PROFILE_FUNCTION();

		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		GE_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, data);
		//glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	inline void OpenGLTexture2D::SetWrapMode(WrapMode mode) 
	{
		GE_PROFILE_FUNCTION();

		m_wrapMode = mode;
		glBindTexture(GL_TEXTURE_2D, m_id);

		switch (mode)
		{
			case WrapMode::Repeat:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case WrapMode::Clamp:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case WrapMode::Mirror:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				//glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
		}
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		GE_PROFILE_FUNCTION();

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_id);
		//glBindTextureUnit(slot, m_id);
	}

}

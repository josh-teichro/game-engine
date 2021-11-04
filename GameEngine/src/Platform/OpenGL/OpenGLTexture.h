#pragma once

#include "GameEngine/Renderer/Texture.h"

#include <glad/glad.h>

namespace GameEngine
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void SetWrapMode(WrapMode mode) override;

		virtual void Bind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override;

	public:
		uint32_t m_id;
		uint32_t m_width, m_height;
		std::string m_filepath;		
		GLenum m_internalFormat, m_dataFormat;
		WrapMode m_wrapMode = WrapMode::Repeat;
	};

}

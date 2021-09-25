#pragma once

#include "GameEngine/Renderer/Texture.h"

namespace GameEngine
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; }
		virtual uint32_t GetHeight() const override { return m_height; }

		virtual void Bind(uint32_t slot) const override;

	public:
		uint32_t m_id;
		uint32_t m_width, m_height;
		std::string m_filepath;
	};

}

#pragma once

#include "GameEngine/Core/Core.h"

namespace GameEngine
{

	/**
	* Texture
	*/
	class Texture
	{
	public:
		enum class WrapMode 
		{
			Repeat,
			Clamp,
			Mirror
		};

	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void SetWrapMode(WrapMode mode) = 0;

		virtual void Bind(uint32_t slot) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	/**
	* Texture2D
	*/
	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const std::string& filepath);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};

}

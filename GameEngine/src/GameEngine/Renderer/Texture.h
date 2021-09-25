#pragma once

#include "GameEngine/Core.h"

namespace GameEngine
{

	/**
	* Texture
	*/
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const= 0;

		virtual void Bind(uint32_t slot) const = 0;
	};

	/**
	* Texture2D
	*/
	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const std::string& filepath);
	};

}

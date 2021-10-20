#pragma once

#include "GameEngine/Renderer/RendererAPI.h"

namespace GameEngine
{

	/**
	* OpenGL rendering API.
	*/
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawArray(const Ref<VertexArray>& vertexArray) override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual RendererAPI::API GetAPI() override { return RendererAPI::API::OpenGL; }

	};

}

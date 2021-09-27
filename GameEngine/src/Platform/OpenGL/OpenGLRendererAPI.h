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

		virtual RendererAPI::API GetAPI() override { return RendererAPI::API::OpenGL; }

	};

}

#pragma once

#include "gepch.h"
#include "VertexArray.h"

#include "glm/glm.hpp"

namespace GameEngine
{

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawArray(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	};

}

#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace GameEngine {

	enum class RenderAPI {
		None = 0, OpenGL = 1
	};

	class Renderer {
	public:
		Renderer();

		void Render();

		static RenderAPI GetAPI() { return m_api; }

	private:
		static RenderAPI m_api;

		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;

		std::shared_ptr<Shader> m_shader2;
		std::shared_ptr<VertexArray> m_vertexArray2;
	};

}

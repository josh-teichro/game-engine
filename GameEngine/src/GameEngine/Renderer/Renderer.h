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

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexArray> m_vertexArray;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
	};

}

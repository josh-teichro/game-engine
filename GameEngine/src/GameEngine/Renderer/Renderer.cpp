#include "gepch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace GameEngine {

	RenderAPI Renderer::m_api = RenderAPI::OpenGL;

	Renderer::Renderer() 
	{
		m_vertexArray.reset(VertexArray::Create());

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.4f, 0.7f, 1.0f
		};

		m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		m_vertexArray->AddBuffer(*m_vertexBuffer, {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec4, "a_color" }
		});

		unsigned int indices[3] = { 0, 1, 2 };
		m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

		m_shader.reset(Shader::Create("../GameEngine/res/shaders/basic.shader"));
	}

	void Renderer::Render() 
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	}

}
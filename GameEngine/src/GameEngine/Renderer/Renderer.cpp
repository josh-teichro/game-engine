#include "gepch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace GameEngine {

	RenderAPI Renderer::m_api = RenderAPI::OpenGL;

	Renderer::Renderer() 
	{
		// 1
		m_vertexArray = VertexArray::Create();

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			 0.45f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 -0.5f,  0.45f, 0.0f, 0.0f, 0.4f, 0.7f, 1.0f
		};

		VertexBufferLayout layout = {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec4, "a_color" }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), layout);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(indexBuffer);


		m_shader = Shader::Create("../GameEngine/res/shaders/red.shader");

		// 2
		m_vertexArray2 = VertexArray::Create();

		float vertices2[3 * 3] = {
			0.5f, 0.5f, 0.0f,
			-0.45f,  0.5f, 0.0f,
			0.5f, -0.45f, 0.0f
		};

		VertexBufferLayout layout2 = {
			{ ShaderDataType::Vec3, "a_position" }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer2 = VertexBuffer::Create(vertices2, sizeof(vertices2), layout2);

		uint32_t indices2[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer2 = IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t));

		m_vertexArray2->AddVertexBuffer(vertexBuffer2);
		m_vertexArray2->SetIndexBuffer(indexBuffer2);

		m_shader2 = Shader::Create("../GameEngine/res/shaders/blue.shader");
	}

	void Renderer::Render() 
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->Bind();
		m_vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

		m_shader2->Bind();
		m_vertexArray2->Bind();
		glDrawElements(GL_TRIANGLES, m_vertexArray2->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
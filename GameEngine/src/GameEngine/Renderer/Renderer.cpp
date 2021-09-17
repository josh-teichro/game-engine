#include "gepch.h"
#include "Renderer.h"

#include <glad/glad.h>

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace GameEngine {

	RenderAPI Renderer::m_api = RenderAPI::OpenGL;

	Renderer::Renderer() 
	{

		float vertices[3 * 3 + 3 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.2f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.4f, 0.7f, 1.0f
		};

		VertexBufferLayout layout = {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec4, "a_color" }
		};

		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices), layout);

		unsigned int indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_vertexArray = VertexArray::Create();
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(indexBuffer);


		m_shader.reset(Shader::Create("../GameEngine/res/shaders/basic.shader"));
	}

	void Renderer::Render() 
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->Bind();
		m_vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}
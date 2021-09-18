#include "gepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GameEngine {

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	std::unique_ptr<RendererAPI> Renderer::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_rendererAPI->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		s_rendererAPI->Clear();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
	{
		vertexArray->Bind();
		shader->Bind();
		s_rendererAPI->DrawArray(vertexArray);
	}

}
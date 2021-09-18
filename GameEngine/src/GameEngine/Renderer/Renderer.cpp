#include "gepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GameEngine {

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	std::unique_ptr<RendererAPI> Renderer::s_rendererAPI = std::make_unique<OpenGLRendererAPI>();
	bool Renderer::s_sceneActive = false;
	std::shared_ptr<Camera> Renderer::s_camera = nullptr;

	void Renderer::BeginScene(const std::shared_ptr<Camera>& camera)
	{
		GE_CORE_ASSERT(!s_sceneActive, "Cannot render two scenes at the same time!");

		s_camera = camera;
		s_camera->ComputeMatrices();
		s_sceneActive = true;
	}

	void Renderer::EndScene()
	{
		s_sceneActive = false;
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		s_rendererAPI->SetClearColor(color);
	}

	void Renderer::Clear()
	{
		s_rendererAPI->Clear();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, const std::shared_ptr<Shader>& shader)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", s_camera->VP() * transform);
		s_rendererAPI->DrawArray(vertexArray);
	}

}
#include "gepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace GameEngine {

	Scope<RendererAPI> Renderer::s_rendererAPI = MakeScope<OpenGLRendererAPI>();
	Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData();
	bool Renderer::s_sceneActive = false;

	void Renderer::BeginScene(const Ref<Camera>& camera)
	{
		GE_CORE_ASSERT(!s_sceneActive, "Cannot render two scenes at the same time!");

		camera->RecalculateMatrices();
		s_sceneData->ViewProjectionMatrix = camera->VP();
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

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, const Ref<Shader>& shader)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", transform * s_sceneData->ViewProjectionMatrix);
		s_rendererAPI->DrawArray(vertexArray);
	}

}
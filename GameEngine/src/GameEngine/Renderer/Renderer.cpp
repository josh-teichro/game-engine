#include "gepch.h"

#include "GameEngine/Renderer/Renderer.h"

#include <glad/glad.h>

namespace GameEngine {

	Scope<Renderer::SceneData> Renderer::s_sceneData = MakeScope<Renderer::SceneData>();
	bool Renderer::s_sceneActive = false;

	void Renderer::Init()
	{
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginScene(const Ref<Camera>& camera)
	{
		GE_CORE_ASSERT(!s_sceneActive, "Cannot render two scenes at the same time!");

		camera->RecalculateMatrices();
		s_sceneData->viewProjectionMatrix = camera->VP();
		s_sceneActive = true;
	}

	void Renderer::EndScene()
	{
		s_sceneActive = false;
	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, const Ref<Shader>& shader)
	{
		vertexArray->Bind();
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", s_sceneData->viewProjectionMatrix * transform.ToMat4());
		RenderCommand::DrawArray(vertexArray);
	}

}
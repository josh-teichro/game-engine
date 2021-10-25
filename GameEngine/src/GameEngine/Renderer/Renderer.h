#pragma once

#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/VertexArray.h"
#include "GameEngine/Renderer/RenderCommand.h"
#include "GameEngine/Renderer/Camera.h"

#include <glm/glm.hpp>

namespace GameEngine {

	class Renderer {
	public:
		using API = RenderCommand::API;

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<Camera>& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Transform& transform, const Ref<Shader>& shader);

		static API GetAPI() { return RenderCommand::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static Scope<SceneData> s_sceneData;
		static bool s_sceneActive;

	};

}

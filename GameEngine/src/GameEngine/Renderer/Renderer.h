#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Camera.h"

#include "glm/glm.hpp"

namespace GameEngine {

	class Renderer {
	public:
		using API = RendererAPI::API;

	public:
		static void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const Transform& transform, const std::shared_ptr<Shader>& shader);

		static API GetAPI() { return s_rendererAPI->GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static std::unique_ptr<RendererAPI> s_rendererAPI;
		static SceneData* s_sceneData;
		static bool s_sceneActive;

	};

}

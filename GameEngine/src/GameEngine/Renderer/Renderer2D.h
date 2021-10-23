#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "Camera.h"

#include "glm/glm.hpp"

namespace GameEngine {

	class Renderer2D {
	public:
		using API = RenderCommand::API;

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<Camera>& camera);
		static void EndScene();

		//static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		//static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const Transform& transform, const glm::vec4& color);

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static Scope<SceneData> s_sceneData;
		static bool s_sceneActive;

	};

}

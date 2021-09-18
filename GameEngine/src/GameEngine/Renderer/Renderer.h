#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "RendererAPI.h"
#include "Camera.h"

#include "glm/glm.hpp"

namespace GameEngine {

	class Renderer {
	public:
		enum class API {
			None = 0, OpenGL = 1
		};

	public:
		static void BeginScene(const std::shared_ptr<Camera>& camera);
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, const std::shared_ptr<Shader>& shader);

		static API GetAPI() { return s_API; }

	private:
		static bool s_sceneActive;
		static API s_API;
		static std::unique_ptr<RendererAPI> s_rendererAPI;

		static std::shared_ptr<Camera> s_camera;

	};

}

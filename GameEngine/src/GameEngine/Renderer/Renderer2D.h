#pragma once

#include "GameEngine/Renderer/Shader.h"
#include "GameEngine/Renderer/VertexArray.h"
#include "GameEngine/Renderer/RenderCommand.h"
#include "GameEngine/Renderer/Camera.h"
#include "GameEngine/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace GameEngine {

	class Renderer2D {
	public:
		using API = RenderCommand::API;

		struct RectTransform
		{
			glm::vec2 position = { 0.0f, 0.0f };
			glm::vec2 size = { 1.0f, 1.0f };
			float rotation = 0.0f;
			float zIndex = 0.0f;
		};

		struct RectTexture
		{
			Ref<Texture2D> baseTexture;
			glm::vec2 textureCoords[2] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 1.0f }
			};
		};

		struct RectMaterial
		{
			RectTexture texture;
			glm::vec2 textureOffset = { 0.0f, 0.0f };
			glm::vec2 textureScale = { 1.0f, 1.0f };

			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		};

		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;
			uint32_t maxTextureSlotsUsed = 0;
		};

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Ref<Camera>& camera);
		static void EndScene();

		static void Flush();

		static void DrawRect(const RectTransform& transform, const RectMaterial& material);

		static API GetAPI() { return RenderCommand::GetAPI(); }

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void Reset();

	};

}

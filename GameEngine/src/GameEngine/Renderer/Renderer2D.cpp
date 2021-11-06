#include "gepch.h"

#include "GameEngine/Renderer/Renderer2D.h"

#include <glad/glad.h>

namespace GameEngine {

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
		float texIndex;
		glm::vec2 texOffset;
		glm::vec2 texScaleFactor;
	};

	struct Renderer2DData {
		static const uint32_t maxQuads = 1000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // TODO: RenderCaps

		static constexpr glm::vec2 quadVertexPositions[] = {
			{ -0.5f, -0.5f },
			{ -0.5f, 0.5f },
			{ 0.5f, -0.5f },
			{ 0.5f, 0.5f }
		};

		Ref<VertexArray> quadVertexArray;
		Ref<VertexBuffer> quadVertexBuffer;
		Ref<Shader> standardShader;

		uint32_t quadIndexCount = 0;
		QuadVertex* quadVertexBufferBase = nullptr;
		QuadVertex* quadVertexBufferPtr = nullptr;

		uint32_t textureCount = 1; // 0 = white texture
		std::array<Ref<Texture2D>, maxTextureSlots> textures;

		glm::mat4 viewProjectionMatrix;

		bool isSceneActive = false;

		Renderer2D::Statistics stats;
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		GE_PROFILE_FUNCTION();

		s_data.quadVertexArray = VertexArray::Create();

		VertexBufferLayout layout = {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec4, "a_color" },
			{ ShaderDataType::Vec2, "a_texCoord" },
			{ ShaderDataType::Float, "a_texIndex" },
			{ ShaderDataType::Vec2, "a_texOffset" },
			{ ShaderDataType::Vec2, "a_texScaleFactor" }
		};

		s_data.quadVertexBuffer = VertexBuffer::Create(s_data.maxVertices * sizeof(QuadVertex), layout);

		s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

		// generate quad indices
		uint32_t quadIndices[s_data.maxIndices];
		int offset = 0;

		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;
			quadIndices[i + 3] = offset + 3;
			quadIndices[i + 4] = offset + 1;
			quadIndices[i + 5] = offset + 2;
			offset += 4;
		}

		Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data.maxIndices);

		s_data.quadVertexArray->AddVertexBuffer(s_data.quadVertexBuffer);
		s_data.quadVertexArray->SetIndexBuffer(quadIndexBuffer);

		s_data.textures[0] = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.textures[0]->SetData(&whiteTextureData, sizeof(uint32_t));

		s_data.standardShader = Shader::Create("./res/shaders/standard.shader");

		s_data.standardShader->Bind();
		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; i++)
			samplers[i] = i;
		s_data.standardShader->SetUniform1iv("u_textures", samplers, s_data.maxTextureSlots);
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_data.quadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Ref<Camera>& camera)
	{
		GE_PROFILE_FUNCTION();

		GE_CORE_ASSERT(!s_data.isSceneActive, "Cannot render two scenes at the same time!");

		camera->RecalculateMatrices();
		s_data.viewProjectionMatrix = camera->VP();

		Reset();

		s_data.isSceneActive = true;
	}

	void Renderer2D::EndScene()
	{
		GE_PROFILE_FUNCTION();

		Flush();

		s_data.isSceneActive = false;
	}

	void Renderer2D::Flush()
	{
		if (s_data.quadIndexCount == 0)
			return;

		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase);
		s_data.quadVertexBuffer->SetData(s_data.quadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_data.textureCount; i++)
		{
			s_data.textures[i]->Bind(i);
		}

		s_data.stats.maxTextureSlotsUsed = s_data.stats.maxTextureSlotsUsed > s_data.textureCount ? s_data.stats.maxTextureSlotsUsed : s_data.textureCount;

		s_data.standardShader->Bind();
		s_data.standardShader->SetUniformMat4f("u_viewProjectionMatrix", s_data.viewProjectionMatrix);

		s_data.quadVertexArray->Bind();
		RenderCommand::DrawArray(s_data.quadVertexArray, dataSize);

		// stats
		s_data.stats.drawCalls++;
	}

	void Renderer2D::DrawRect(const RectTransform& transform, const RectMaterial& material)
	{
		GE_PROFILE_FUNCTION();

		if (s_data.quadIndexCount >= s_data.maxIndices) {
			Flush();
			Reset();
		}

		glm::vec4 color = material.color;
		float texIndex = 0.0f;
		glm::vec2 texOffset = material.textureOffset;
		glm::vec2 texScaleFactor = 1.0f / material.textureScale;

		if (material.texture.baseTexture)
		{
			for (uint32_t i = 0; i < s_data.textureCount; i++)
			{
				if (s_data.textures[i] == material.texture.baseTexture)
				{
					texIndex = (float)i;
					break;
				}
			}

			if (texIndex == 0)
			{
				if (s_data.textureCount >= s_data.maxTextureSlots)
				{
					Flush();
					Reset();
				}

				s_data.textures[s_data.textureCount] = material.texture.baseTexture;
				texIndex = (float)s_data.textureCount;
				s_data.textureCount++;
			}
		}

		glm::vec2 texCoords[] = {
			{material.texture.textureCoords[0][0], material.texture.textureCoords[0][1]},
			{material.texture.textureCoords[0][0], material.texture.textureCoords[1][1]},
			{material.texture.textureCoords[1][0], material.texture.textureCoords[0][1]},
			{material.texture.textureCoords[1][0], material.texture.textureCoords[1][1]}
		};

		for (uint32_t i = 0; i < 4; i++) {

			if (transform.rotation == 0.0f) {
				s_data.quadVertexBufferPtr[i].position = glm::vec3(transform.position + s_data.quadVertexPositions[i] * transform.size, transform.zIndex);
			}
			else {
				s_data.quadVertexBufferPtr[i].position = {
					transform.position.x + (s_data.quadVertexPositions[i].x * transform.size.x * cos(transform.rotation) - (s_data.quadVertexPositions[i].y * transform.size.y) * sin(transform.rotation)),
					transform.position.y + (s_data.quadVertexPositions[i].x * transform.size.x * sin(transform.rotation) + (s_data.quadVertexPositions[i].y * transform.size.y) * cos(transform.rotation)),
					transform.zIndex
				};
			}

			s_data.quadVertexBufferPtr[i].color = color;
			s_data.quadVertexBufferPtr[i].texCoord = texCoords[i];
			s_data.quadVertexBufferPtr[i].texIndex = texIndex;
			s_data.quadVertexBufferPtr[i].texOffset = texOffset;
			s_data.quadVertexBufferPtr[i].texScaleFactor = texScaleFactor;
		}

		s_data.quadVertexBufferPtr += 4;
		s_data.quadIndexCount += 6;

		// stats
		s_data.stats.quadCount++;
	}

	void Renderer2D::ResetStats()
	{
		s_data.stats.drawCalls = 0;
		s_data.stats.quadCount = 0;
		s_data.stats.vertexCount = 0;
		s_data.stats.indexCount = 0;
		s_data.stats.maxTextureSlotsUsed = 0;
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		s_data.stats.vertexCount = s_data.stats.quadCount * 4;
		s_data.stats.indexCount = s_data.stats.quadCount * 6;
		return s_data.stats;
	}

	void Renderer2D::Reset()
	{
		s_data.textureCount = 1;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
		s_data.quadIndexCount = 0;
	}

}
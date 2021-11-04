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
		static const uint32_t maxQuads = 100;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32; // TODO: RenderCaps

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
		uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase);
		s_data.quadVertexBuffer->SetData(s_data.quadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_data.textureCount; i++)
		{
			s_data.textures[i]->Bind(i);
		}

		s_data.standardShader->Bind();
		s_data.standardShader->SetUniformMat4f("u_viewProjectionMatrix", s_data.viewProjectionMatrix);

		s_data.quadVertexArray->Bind();
		RenderCommand::DrawArray(s_data.quadVertexArray, dataSize);
	}

	void Renderer2D::DrawRect(const RectTransform& transform, const RectMaterial& material)
	{
		GE_PROFILE_FUNCTION();

		glm::vec4 color = material.color;
		float texIndex = 0.0f;
		glm::vec2 texOffset = material.textureOffset;
		glm::vec2 texScaleFactor = 1.0f / material.textureScale;

		if (material.texture)
		{
			for (uint32_t i = 0; i < s_data.textureCount; i++)
			{
				if (s_data.textures[i] == material.texture)
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

				s_data.textures[s_data.textureCount] = material.texture;
				texIndex = (float)s_data.textureCount;
				s_data.textureCount++;
			}
		}

		if (transform.rotation == 0.0f) {
			s_data.quadVertexBufferPtr[0].position = { transform.position.x - 0.5f * transform.size.x, transform.position.y - 0.5f * transform.size.y, transform.zIndex };
			s_data.quadVertexBufferPtr[1].position = { transform.position.x - 0.5f * transform.size.x, transform.position.y + 0.5f * transform.size.y, transform.zIndex };
			s_data.quadVertexBufferPtr[2].position = { transform.position.x + 0.5f * transform.size.x, transform.position.y - 0.5f * transform.size.y, transform.zIndex };
			s_data.quadVertexBufferPtr[3].position = { transform.position.x + 0.5f * transform.size.x, transform.position.y + 0.5f * transform.size.y, transform.zIndex };
		}
		else {
			s_data.quadVertexBufferPtr[0].position = { 
				transform.position.x + ((-0.5f) * transform.size.x * cos(transform.rotation) - (-0.5f * transform.size.y) * sin(transform.rotation)),
				transform.position.y + ((-0.5f) * transform.size.x * sin(transform.rotation) + (-0.5f * transform.size.y) * cos(transform.rotation)),
				transform.zIndex 
			};
			s_data.quadVertexBufferPtr[1].position = {
				transform.position.x + ((-0.5f) * transform.size.x * cos(transform.rotation) - (0.5f) * transform.size.y * sin(transform.rotation)),
				transform.position.y + ((-0.5f) * transform.size.x * sin(transform.rotation) + (0.5f) * transform.size.y * cos(transform.rotation)),
				transform.zIndex
			};
			s_data.quadVertexBufferPtr[2].position = {
				transform.position.x + ((0.5f) * transform.size.x * cos(transform.rotation) - (-0.5f) * transform.size.y * sin(transform.rotation)),
				transform.position.y + ((0.5f) * transform.size.x * sin(transform.rotation) + (-0.5f) * transform.size.y * cos(transform.rotation)),
				transform.zIndex
			};
			s_data.quadVertexBufferPtr[3].position = {
				transform.position.x + ((0.5f) * transform.size.x * cos(transform.rotation) - (0.5f) * transform.size.y * sin(transform.rotation)),
				transform.position.y + ((0.5f) * transform.size.x * sin(transform.rotation) + (0.5f) * transform.size.y * cos(transform.rotation)),
				transform.zIndex
			};
		}

		s_data.quadVertexBufferPtr[0].color = color;
		s_data.quadVertexBufferPtr[0].texCoord = { 0.0f, 0.0f };
		s_data.quadVertexBufferPtr[0].texIndex = texIndex;
		s_data.quadVertexBufferPtr[0].texOffset = texOffset;
		s_data.quadVertexBufferPtr[0].texScaleFactor = texScaleFactor;

		s_data.quadVertexBufferPtr[1].color = color;
		s_data.quadVertexBufferPtr[1].texCoord = { 0.0f, 1.0f };
		s_data.quadVertexBufferPtr[1].texIndex = texIndex;
		s_data.quadVertexBufferPtr[1].texOffset = texOffset;
		s_data.quadVertexBufferPtr[1].texScaleFactor = texScaleFactor;

		s_data.quadVertexBufferPtr[2].color = color;
		s_data.quadVertexBufferPtr[2].texCoord = { 1.0f, 0.0f };
		s_data.quadVertexBufferPtr[2].texIndex = texIndex;
		s_data.quadVertexBufferPtr[2].texOffset = texOffset;
		s_data.quadVertexBufferPtr[2].texScaleFactor = texScaleFactor;

		s_data.quadVertexBufferPtr[3].color = color;
		s_data.quadVertexBufferPtr[3].texCoord = { 1.0f, 1.0f };
		s_data.quadVertexBufferPtr[3].texIndex = texIndex;
		s_data.quadVertexBufferPtr[3].texOffset = texOffset;
		s_data.quadVertexBufferPtr[3].texScaleFactor = texScaleFactor;

		s_data.quadVertexBufferPtr += 4;
	}

	void Renderer2D::Reset()
	{
		s_data.textureCount = 1;
		s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
	}

}
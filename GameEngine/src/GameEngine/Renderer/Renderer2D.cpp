#include "gepch.h"
#include "Renderer2D.h"

#include <glad/glad.h>

namespace GameEngine {

	Scope<Renderer2D::SceneData> Renderer2D::s_sceneData = MakeScope<Renderer2D::SceneData>();
	bool Renderer2D::s_sceneActive = false;

	struct Renderer2DStorage {
		Ref<VertexArray> squareVertexArray;
		Ref<Shader> standardShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage();

		s_data->squareVertexArray = VertexArray::Create();

		float vertices[4 * (3 + 2)] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f
		};

		VertexBufferLayout layout = {
			{ ShaderDataType::Vec3, "a_position" },
			{ ShaderDataType::Vec2, "a_texCoord" }
		};

		Ref<VertexBuffer> vertexBuffer3 = VertexBuffer::Create(vertices, sizeof(vertices), layout);

		uint32_t indices[6] = { 0, 1, 2, 3, 1, 2 };
		Ref<IndexBuffer> indexBuffer3 = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		s_data->squareVertexArray->AddVertexBuffer(vertexBuffer3);
		s_data->squareVertexArray->SetIndexBuffer(indexBuffer3);

		s_data->standardShader = Shader::Create("./res/shaders/standard.shader");
	}

	void Renderer2D::Shutdown()
	{
		delete s_data;
	}

	void Renderer2D::BeginScene(const Ref<Camera>& camera)
	{
		GE_CORE_ASSERT(!s_sceneActive, "Cannot render two scenes at the same time!");

		camera->RecalculateMatrices();
		s_sceneData->viewProjectionMatrix = camera->VP();

		s_data->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_data->standardShader->Bind();		
		s_data->standardShader->SetUniform1i("u_texture", 0);

		s_sceneActive = true;
	}

	void Renderer2D::EndScene()
	{
		s_sceneActive = false;
	}

	void Renderer2D::DrawRect(const Transform& transform, const glm::vec4& color)
	{
		s_data->whiteTexture->Bind(0);

		s_data->standardShader->SetUniformMat4f("u_MVP", s_sceneData->viewProjectionMatrix * transform.ToMat4());
		s_data->standardShader->SetUniform4f("u_color", color);

		s_data->squareVertexArray->Bind();
		RenderCommand::DrawArray(s_data->squareVertexArray);
	}

	void Renderer2D::DrawRect(const Transform& transform, const Ref<Texture2D>& texture)
	{
		texture->Bind(0);

		s_data->standardShader->SetUniformMat4f("u_MVP", s_sceneData->viewProjectionMatrix * transform.ToMat4());
		s_data->standardShader->SetUniform4f("u_color", glm::vec4(1.0f));

		s_data->squareVertexArray->Bind();
		RenderCommand::DrawArray(s_data->squareVertexArray);	
	}

}
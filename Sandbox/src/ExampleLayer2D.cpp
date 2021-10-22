#include "ExampleLayer2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

ExampleLayer2D::ExampleLayer2D() : 
	m_cameraController(1.0f)
{
	CreateScene();
}

void ExampleLayer2D::OnAttach()
{
	ResetScene();
}

void ExampleLayer2D::OnDetach()
{
}

void ExampleLayer2D::OnUpdate()
{
	float deltaTime = GameEngine::Time::GetDeltaTime();

	m_cameraController.OnUpdate();

	// draw scene
	GameEngine::Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::Renderer::Clear();

	GameEngine::Renderer::BeginScene(m_cameraController.GetCamera());

	auto flatShader = m_shaderLibrary.Get("flat");
	flatShader->SetUniform4f("u_color", m_squareColor);

	GameEngine::Renderer::Submit(m_square, m_squareTransform, flatShader);

	GameEngine::Renderer::EndScene();
}

void ExampleLayer2D::OnImGuiUpdate()
{
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

	ImGui::BeginDisabled(m_walk);
	if (ImGui::Button("Walk"))
	{
		m_walk = true;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Walk);
	}
	ImGui::EndDisabled();

	if (ImGui::Button("Reset"))
	{
		ResetScene();
	}
}

bool ExampleLayer2D::OnEvent(const GameEngine::Event& e)
{
	bool result = false;
	result |= m_cameraController.OnEvent(e);
	result |= HandlesEvents::OnEvent(e);
	return result;
}

bool ExampleLayer2D::OnKeyUp(const GameEngine::KeyUpEvent& e)
{
	if (e.keyCode == GameEngine::KeyCode::Escape)
	{
		m_walk = false;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Normal);
	}
	else if (e.keyCode == GameEngine::KeyCode::W)
	{
		m_walk = true;
		m_cameraController.SetMode(GameEngine::CameraController2D::Mode::Walk);
	}

	return false;
}

void ExampleLayer2D::CreateScene()
{
	m_square = GameEngine::VertexArray::Create();

	float vertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	GameEngine::VertexBufferLayout layout = {
		{ GameEngine::ShaderDataType::Vec3, "a_position" }
	};

	GameEngine::Ref<GameEngine::VertexBuffer> vertexBuffer3 = GameEngine::VertexBuffer::Create(vertices, sizeof(vertices), layout);

	uint32_t indices[6] = { 0, 1, 2, 3, 1, 2 };
	GameEngine::Ref<GameEngine::IndexBuffer> indexBuffer3 = GameEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

	m_square->AddVertexBuffer(vertexBuffer3);
	m_square->SetIndexBuffer(indexBuffer3);

	m_shaderLibrary.Load("./res/shaders/flat.shader");
}

void ExampleLayer2D::ResetScene()
{
	m_cameraController.SetZoom(1.0f);
	m_cameraController.SetPosition({ 0.0f, 0.5f });

	m_squareTransform.position = { 0.0f, 0.5f, 0.0f };
	m_squareColor = { 0.2f, 0.1f, 0.7f, 1.0f };
}

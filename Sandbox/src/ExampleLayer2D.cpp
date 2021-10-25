#include "ExampleLayer2D.h"

#include <imgui.h>
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
	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::RenderCommand::Clear();

	GameEngine::Renderer2D::BeginScene(m_cameraController.GetCamera());

	m_squareTransform.SetEulerAngles(m_squareRotation);
	GameEngine::Renderer2D::DrawRect(m_squareTransform, m_squareColor);
	GameEngine::Renderer2D::DrawRect(m_backgroundTransform, m_backgroundTexture);

	GameEngine::Renderer2D::EndScene();
}

void ExampleLayer2D::OnImGuiUpdate()
{
	ImGui::DragFloat3("Square Position", glm::value_ptr(m_squareTransform.position));
	ImGui::DragFloat3("Square Rotation", glm::value_ptr(m_squareRotation));
	ImGui::DragFloat3("Square Scale", glm::value_ptr(m_squareTransform.scale));
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
	m_backgroundTexture = GameEngine::Texture2D::Create("./res/textures/checkerboard.png");
	m_backgroundTexture->SetWrapMode(GameEngine::Texture::WrapMode::Repeat);
	m_backgroundTransform.position.z = -0.1f;
	m_backgroundTransform.scale = { 10.0f, 10.0f, 10.0f };
}

void ExampleLayer2D::ResetScene()
{
	m_cameraController.SetZoom(1.0f);
	m_cameraController.SetPosition({ 0.0f, 0.0f });

	m_squareTransform.position = { 0.0f, 0.0f, 0.0f };
	m_squareTransform.scale = { 1.0f, 1.0f, 1.0f };
	m_squareRotation = { 0.0f, 0.0f, 0.0f };
	m_squareColor = { 0.3f, 0.2f, 0.8f, 1.0f };
}

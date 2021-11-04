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
	GE_PROFILE_FUNCTION();

	float deltaTime = GameEngine::Time::GetDeltaTime();

	m_cameraController.OnUpdate();

	// draw scene
	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	GameEngine::RenderCommand::Clear();

	GameEngine::Renderer2D::BeginScene(m_cameraController.GetCamera());

	m_squareTransform.rotation = glm::radians(m_squareRotationDeg);
	GameEngine::Renderer2D::DrawRect(m_squareTransform, m_squareMaterial);
	GameEngine::Renderer2D::DrawRect(m_backgroundTransform, m_backgroundMaterial);

	GameEngine::Renderer2D::EndScene();
}

void ExampleLayer2D::OnImGuiUpdate()
{
	GE_PROFILE_FUNCTION();

	ImGui::DragFloat2("Square Position", glm::value_ptr(m_squareTransform.position), 0.1f);
	ImGui::DragFloat("Square Rotation", &m_squareRotationDeg, 1.0f, 0.0f, 360.0f);
	ImGui::DragFloat2("Square Size", glm::value_ptr(m_squareTransform.size), 0.1f, 0.0f);
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareMaterial.color));

	ImGui::DragFloat2("Background Tiling Offset", &m_backgroundMaterial.textureOffset[0], 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("Background Tiling Scale", &m_backgroundMaterial.textureScale[0], 0.01f, 0.0f, 10.0f);

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
	GE_PROFILE_FUNCTION();

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
	GE_PROFILE_FUNCTION();

	m_backgroundMaterial.texture = GameEngine::Texture2D::Create("./res/textures/checkerboard.png");
	m_backgroundMaterial.texture->SetWrapMode(GameEngine::Texture::WrapMode::Repeat);
	m_backgroundTransform.zIndex = -0.1f;
	m_backgroundTransform.size = { 10.0f, 10.0f };
}

void ExampleLayer2D::ResetScene()
{
	GE_PROFILE_FUNCTION();

	m_cameraController.SetZoom(1.0f);
	m_cameraController.SetPosition({ 0.0f, 0.0f });

	m_squareTransform.position = { 0.0f, 0.0f };
	m_squareTransform.size = { 1.0f, 1.0f };
	m_squareTransform.rotation = m_squareRotationDeg = 0.0f;
	m_squareMaterial.color = { 0.3f, 0.2f, 0.8f, 1.0f };
}
